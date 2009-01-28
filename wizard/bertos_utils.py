#!/usr/bin/env python
# encoding: utf-8
#
# Copyright 2008 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# $Id:$
#
# Author: Lorenzo Berni <duplo@develer.com>
#

import os
import fnmatch
import glob
import re
import shutil

import const

def isBertosDir(directory):
   return os.path.exists(directory + "/VERSION")

def bertosVersion(directory):
   return open(directory + "/VERSION").readline().strip()

def createBertosProject(directory, sourcesDir, projectInfos):
    if not os.path.isdir(directory):
        os.mkdir(directory)
    f = open(directory + "/project.bertos", "w")
    f.write(repr(projectInfos))
    f.close()
    ## Destination source dir
    srcdir = directory + "/bertos"
    shutil.rmtree(srcdir, True)
    shutil.copytree(sourcesDir + "/bertos", srcdir)
    ## Destination makefile
    makefile = directory + "/Makefile"
    if os.path.exists(makefile):
        os.remove(makefile)
    shutil.copy(sourcesDir + "/Makefile", makefile)
    ## Destination project dir
    prjdir = directory + "/" + os.path.basename(directory)
    shutil.rmtree(prjdir, True)
    os.mkdir(prjdir)

def getSystemPath():
    path = os.environ["PATH"]
    if os.name == "nt":
        path = path.split(";")
    else:
        path = path.split(":")
    return path

def findToolchains(pathList):
    toolchains = []
    for element in pathList:
        for toolchain in glob.glob(element+ "/" + const.GCC_NAME):
            if not os.path.islink(toolchain):
                toolchains.append(toolchain)
    return list(set(toolchains))

def getToolchainInfo(output):
    info = {}
    expr = re.compile("Target: .*")
    target = expr.findall(output)
    if len(target) == 1:
        info["target"] = target[0].split("Target: ")[1]
    expr = re.compile("gcc version [0-9,.]*")
    version = expr.findall(output)
    if len(version) == 1:
        info["version"] = version[0].split("gcc version ")[1]
    expr = re.compile("gcc version [0-9,.]* \(.*\)")
    build = expr.findall(output)
    if len(build) == 1:
        build = build[0].split("gcc version ")[1]
        build = build[build.find("(") + 1 : build.find(")")]
        info["build"] = build
    expr = re.compile("Configured with: .*")
    configured = expr.findall(output)
    if len(configured) == 1:
        info["configured"] = configured[0].split("Configured with: ")[1]
    expr = re.compile("Thread model: .*")
    thread = expr.findall(output)
    if len(thread) == 1:
        info["thread"] = thread[0].split("Thread model: ")[1]
    return info

def findDefinitions(ftype, path):
    L = os.walk(path)
    for element in L:
        for filename in element[2]:
            if fnmatch.fnmatch(filename, ftype):
                yield (filename, element[0])

def loadCpuInfos(path):
    cpuInfos = []
    for definition in findDefinitions(const.CPU_DEFINITION, path):
        cpuInfos.append(getInfos(definition))
    return cpuInfos

def getInfos(definition):
    D = {}
    D.update(const.CPU_DEF)
    def include(filename, dict = D, directory=definition[1]):
        execfile(directory + "/" + filename, {}, D)
    D["include"] = include
    include(definition[0], D)
    D["CPU_NAME"] = definition[0].split(".")[0]
    D["DEFINITION_PATH"] = definition[1] + "/" + definition[0]
    del D["include"]
    return D

def getDefinitionBlocks(text):
    """
    Take a text and return a list of tuple (description, name-value).
    """
    block = []
    block_tmp = re.findall(r"/\*{2}\s*([^*]*\*(?:[^/*][^*]*\*+)*)/\s*#define\s+((?:[^/]*?/?)+)\s*?(?:/{2,3}[^<].*?)?$", text, re.MULTILINE)
    for comment, define in block_tmp:
        block.append((" ".join(re.findall(r"^\s*\*?\s*(.*?)\s*?(?:/{2}.*?)?$", comment, re.MULTILINE)).strip(), define))
    block += re.findall(r"/{3}\s*([^<].*?)\s*#define\s+((?:[^/]*?/?)+)\s*?(?:/{2,3}[^<].*?)?$", text, re.MULTILINE)
    block += [(comment, define) for define, comment in re.findall(r"#define\s*(.*?)\s*/{3}<\s*(.+?)\s*?(?:/{2,3}[^<].*?)?$", text, re.MULTILINE)]
    return block

def formatParamNameValue(text):
    """
    Take the given string and return a tuple with the name of the parameter in the first position
    and the value in the second.
    """
    block = re.findall("\s*([^\s]+)\s*(.+?)\s*$", text, re.MULTILINE)
    return block[0]

def getDescriptionInformations(text): 
    """ 
    Take the doxygen comment and strip the wizard informations, returning the tuple 
    (comment, wizard_informations) 
    """ 
    index = text.find("$WIZARD") 
    if index != -1: 
        exec(text[index + 1:]) 
        informations = WIZARD 
        return text[:index].strip(), informations
    else:
        return text.strip(), {}

def loadConfigurationInfos(path):
    """
    Return the module configurations found in the given file as a dict with the
    parameter name as key and a dict containig the fields above as value:
        "value": the value of the parameter
        "description": the description of the parameter
        "informations": a dict containig optional informations:
            "type": "int" | "boolean" | "enum"
            "min": the minimum value for integer parameters
            "max": the maximum value for integer parameters
            "long": boolean indicating if the num is a long
            "value_list": the name of the enum for enum parameters
    """
    configurationInfos = {}
    for comment, define in getDefinitionBlocks(open(path, "r").read()):
        name, value = formatParamNameValue(define)
        description, informations = getDescriptionInformations(comment)
        configurationInfos[name] = {}
        configurationInfos[name]["value"] = value
        configurationInfos[name]["informations"] = informations
        configurationInfos[name]["description"] = description
    return configurationInfos

def loadModuleInfos(path):
    """
    Return the module infos found in the given file as a dict with the module
    name as key and a dict containig the fields above as value or an empty dict
    if the given file is not a BeRTOS module:
        "depends": a list of modules needed by this module
        "configuration": the cfg_*.h with the module configurations
        "description": a string containing the brief description of doxygen
        "enabled": contains False but the wizard will change if the user select
        the module
    """
    moduleInfos = {}
    string = open(path, "r").read()
    commentList = re.findall(r"/\*{2}\s*([^*]*\*(?:[^/*][^*]*\*+)*)/", string)
    commentList = [" ".join(re.findall(r"^\s*\*?\s*(.*?)\s*?(?:/{2}.*?)?$", comment, re.MULTILINE)).strip() for comment in commentList]
    for comment in commentList:
        index = comment.find("$WIZARD_MODULE")
        if index != -1:
            exec(comment[index + 1:])
            moduleInfos[WIZARD_MODULE["name"]] = {"depends": WIZARD_MODULE["depends"],
                                                    "configuration": WIZARD_MODULE["configuration"],
                                                    "description": "",
                                                    "enabled": False}
            return moduleInfos
    return {}

def loadModuleInfosDict(path):
    """
    Return the dict containig all the modules
    """
    moduleInfosDict = {}
    for filename, path in findDefinitions("*.h", path):
        moduleInfosDict.update(loadModuleInfos(path + "/" + filename))
    return moduleInfosDict

def loadDefineLists(path):
    """
    Return a dict with the name of the list as key and a list of string as value
    """
    string = open(path, "r").read()
    commentList = re.findall(r"/\*{2}\s*([^*]*\*(?:[^/*][^*]*\*+)*)/", string)
    commentList = [" ".join(re.findall(r"^\s*\*?\s*(.*?)\s*?(?:/{2}.*?)?$", comment, re.MULTILINE)).strip() for comment in commentList]
    listDict = {}
    for comment in commentList:
        index = comment.find("$WIZARD_LIST")
        if index != -1:
            exec(comment[index + 1:])
            listDict.update(WIZARD_LIST)
    return listDict

def loadDefineListsDict(path):
    """
    Return the dict containing all the define lists
    """
    defineListsDict = {}
    for filename, path in findDefinitions("*.h", path):
        defineListsDict.update(loadDefineLists(path + "/" + filename))
    return defineListsDict

def sub(string, parameter, value):
    """
    Substitute the given value at the given parameter define in the given string
    """
    return re.sub(r"(?P<define>#define\s+" + parameter + r"\s+)([^\s]+)", r"\g<define>" + value, string)
