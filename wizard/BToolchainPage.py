#!/usr/bin/env python
# encoding: utf-8
#
# This file is part of BeRTOS.
#
# Bertos is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# As a special exception, you may use this file as part of a free software
# library without restriction.  Specifically, if other files instantiate
# templates or use macros or inline functions from this file, or you compile
# this file and link it with other files to produce an executable, this
# file does not by itself cause the resulting executable to be covered by
# the GNU General Public License.  This exception does not however
# invalidate any other reasons why the executable file might be covered by
# the GNU General Public License.
#
# Copyright 2008 Develer S.r.l. (http://www.develer.com/)
#
# $Id$
#
# Author: Lorenzo Berni <duplo@develer.com>
#

import os
import collections

from BWizardPage import *
from BOutputPage import BOutputPage

import BToolchainSearch
import bertos_utils
import qvariant_converter

from toolchain_manager import ToolchainManager

from const import *

class BToolchainPage(BWizardPage):
    """
    Page of the wizard that permits to choose the toolchain to use for the
    project.
    """

    def __init__(self):
        BWizardPage.__init__(self, UI_LOCATION + "/toolchain_select.ui")
        self.setTitle(self.tr("Select toolchain"))
        self._validation_process = None
        self._valid_items = []
        self._toolchain_manager = ToolchainManager()

    ## Overloaded QWizardPage methods. ##

    def isComplete(self):
        """
        Overload of the QWizard isComplete method.
        """
        if self.pageContent.toolchainList.currentRow() != -1:
            self.setProjectInfo("TOOLCHAIN",
                qvariant_converter.getStringDict(self.pageContent.toolchainList.currentItem().data(Qt.UserRole)))
            return True
        else:
            return False

    def nextId(self):
        """
        Overload of the QWizardPage nextId method.
        """
        # Route to Output page if it's a predefined easy project.
        if self.projectInfo("PROJECT_FROM_PRESET") and self.projectInfo("BASE_MODE"):
            return self.wizard().pageIndex(BOutputPage)
        else:
            return QWizardPage.nextId(self)

    ####

    ## Overloaded BWizardPage methods. ##

    def setupUi(self):
        """
        Sets up the user interface.
        """
        self.pageContent.infoLabel.setVisible(False)

    def connectSignals(self):
        """
        Connects the signals with the related slots.
        """
        self.connect(self.pageContent.toolchainList, SIGNAL("itemSelectionChanged()"), self.selectionChanged)
        self.connect(self.pageContent.addButton, SIGNAL("clicked()"), self.addToolchain)
        self.connect(self.pageContent.removeButton, SIGNAL("clicked()"), self.removeToolchain)
        self.connect(self.pageContent.searchButton, SIGNAL("clicked()"), self.searchToolchain)
        self.connect(self.pageContent.checkButton, SIGNAL("clicked()"), self.validateAllToolchains)

    def reloadData(self):
        """
        Overload of the BWizard reloadData method.
        """
        self._clearList()
        self.setupUi()
        self._populateToolchainList()
        if len(self._valid_items) == 1:
            self.pageContent.toolchainList.setCurrentItem(self._valid_items[0])

    ####

    ## Slots ##

    def selectionChanged(self):
        """
        Slot called when the user click on an entry of the toolchain list.
        """
        if self.pageContent.toolchainList.currentRow() != -1:
            infos = collections.defaultdict(lambda: unicode("not defined"))
            infos.update(qvariant_converter.getStringDict(self.pageContent.toolchainList.currentItem().data(Qt.UserRole)))
            self.pageContent.infoLabel.setText("GCC " + infos["version"] + " (" + infos["build"] + ")\nTarget: " + infos["target"] + "\nPath: " + os.path.normpath(infos["path"]))
            self.pageContent.infoLabel.setVisible(True)
            if self.isDefaultToolchain(infos):
                self.disableRemoveButton()
            else:
                self.enableRemoveButton()
            self.emit(SIGNAL("completeChanged()"))

    def addToolchain(self):
        """
        Slot called when the user adds manually a toolchain.
        """
        sel_toolchain = unicode(QFileDialog.getOpenFileName(self, self.tr("Choose the toolchain"), ""))
        if sel_toolchain != "":
            item = QListWidgetItem(sel_toolchain)
            item.setData(Qt.UserRole, qvariant_converter.convertStringDict({"path": sel_toolchain}))
            self.pageContent.toolchainList.addItem(item)
            self._toolchain_manager.addToolchain(sel_toolchain)

    def removeToolchain(self):
        """
        Slot called when the user removes manually a toolchain.
        """
        if self.pageContent.toolchainList.currentRow() != -1:
            item = self.pageContent.toolchainList.takeItem(self.pageContent.toolchainList.currentRow())
            toolchain = qvariant_converter.getStringDict(item.data(Qt.UserRole))["path"]
            self._toolchain_manager.removeToolchain(toolchain)

    def searchToolchain(self):
        """
        Slot called when the user clicks on the 'search' button. It opens the
        toolchain search dialog.
        """
        search = BToolchainSearch.BToolchainSearch()
        self.connect(search, SIGNAL("accepted()"), self._search)
        search.exec_()

    def validateAllToolchains(self):
        """
        Slot called when the user clicks on the validate button. It starts the
        toolchain validation procedure for all the toolchains.
        """
        try:
            QApplication.instance().setOverrideCursor(Qt.WaitCursor)
            for i in range(self.pageContent.toolchainList.count()):
                self.validateToolchain(i)
        finally:
            QApplication.instance().restoreOverrideCursor()

    ####

    def _populateToolchainList(self):
        """
        Fills the toolchain list with the toolchains stored in the QSettings.
        """
        self.pageContent.toolchainList.clear()
        toolchains = self._toolchain_manager.predefined_toolchains + self._toolchain_manager.toolchains
        sel_toolchain = self.projectInfo("TOOLCHAIN")
        for key, value in toolchains:
            if os.path.exists(key):
                item = QListWidgetItem(key)
                item_data = {"path":key}
                if value:
                    item_data.update(value)
                item.setData(Qt.UserRole, qvariant_converter.convertStringDict(item_data))
                self.pageContent.toolchainList.addItem(item)
                if sel_toolchain and sel_toolchain["path"] == key:
                    self.pageContent.toolchainList.setCurrentItem(item)
                if value is not None:
                    self.validateToolchain(self.pageContent.toolchainList.row(item))

    def currentToolchain(self):
        selected_toolchain = qvariant_converter.getStringDict(self.pageContent.toolchainList.currentItem().data(Qt.UserRole))
        return selected_toolchain

    def _clearList(self):
        """
        Removes all the toolchain from the list.
        """
        self.pageContent.toolchainList.clear()

    def _search(self):
        """
        Searches for toolchains in the stored directories, and stores them in the
        QSettings.
        """
        dir_list = self.searchDirList()
        if self.pathSearch():
            dir_list += [element for element in bertos_utils.getSystemPath()]
        _toolchain_dict = self._toolchain_manager.storedToolchainDict()
        toolchain_list = bertos_utils.findToolchains(dir_list)
        for toolchain in toolchain_list:
            self._toolchain_manager.addToolchain(toolchain, _toolchain_dict.get(toolchain, False))
        self._populateToolchainList()
        self.showMessage(self.tr("Toolchain search result."), self.tr("%1 toolchains founded").arg(len(self._toolchain_manager.toolchains)))

    def _validItem(self, index, infos):
        """
        Sets the item at index as a valid item and associates the given info to it.
        """
        item = self.pageContent.toolchainList.item(index)
        new_data = qvariant_converter.getStringDict(self.pageContent.toolchainList.item(index).data(Qt.UserRole))
        new_data.update(infos)
        item.setData(Qt.UserRole, qvariant_converter.convertStringDict(new_data))
        needed = self.projectInfo("CPU_INFOS")
        if "target" in infos and infos["target"].find(needed["TOOLCHAIN"]) != -1:
            item.setIcon(QIcon(":/images/ok.png"))
            self._valid_items.append(item)
        else:
            item.setIcon(QIcon(":/images/warning.png"))
        if "version" in infos and "target" in infos:
            item.setText("GCC " + infos["version"] + " - " + infos["target"].strip())

    def _invalidItem(self, index):
        """
        Sets the item at index as an invalid item.
        """
        item = self.pageContent.toolchainList.item(index)
        item.setIcon(QIcon(":/images/error.png"))

    def validateToolchain(self, i):
        """
        Toolchain validation procedure.
        """
        filename = qvariant_converter.getStringDict(self.pageContent.toolchainList.item(i).data(Qt.UserRole))["path"]
        info = self._toolchain_manager.validateToolchain(filename)

        # Add the item in the list with the appropriate associate data.
        if info:
            self._validItem(i, info)
        else:
            self._invalidItem(i)
    
    def isDefaultToolchain(self, toolchain):
        """
        Returns True if the given toolchain is one of the default toolchains.
        """
        return toolchain["path"] in self._toolchain_manager._predefined_toolchain_set
    
    def disableRemoveButton(self):
        """
        Disable the remove button.
        """
        self.pageContent.removeButton.setEnabled(False)
    
    def enableRemoveButton(self):
        """
        Enable the remove button.
        """
        self.pageContent.removeButton.setEnabled(True)
        
    def currentItem(self):
        return self.pageContent.toolchainList.currentItem()
