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

from PyQt4.QtCore import *
from PyQt4.QtGui import *
import PyQt4.uic as uic

class BToolchainSearch(QDialog):
    
    def __init__(self):
        QDialog.__init__(self)
        self._setupUi()
        self._connectSignals()
        self.setWindowTitle(self.tr("Toolchain search page"))
    
    def _setupUi(self):
        self.content = uic.loadUi("toolchain_search.ui", None)
        layout = QVBoxLayout()
        layout.addWidget(self.content)
        self.setLayout(layout)
        self._populateDirList()
        self._setPathSearch()
        self._setSearchButton()
    
    def _connectSignals(self):
        self.connect(self.content.pathBox, SIGNAL("stateChanged(int)"), self._stateChanged)
        self.connect(self.content.addButton, SIGNAL("clicked()"), self._addDir)
        self.connect(self.content.removeButton, SIGNAL("clicked()"), self._removeDir)
        self.connect(self.content.cancelButton, SIGNAL("clicked()"), self.reject)
        self.connect(self.content.searchButton, SIGNAL("clicked()"), self.accept)
        
    def _setSearchButton(self):
        self.content.searchButton.setDefault(True)
        self.content.searchButton.setEnabled(self.content.pathBox.isChecked() or self.content.customDirList.count() != 0)
    
    def _populateDirList(self):
        search_dir_list = QApplication.instance().settings.value(QString("search_dir_list")).toList()
        for element in search_dir_list:
            item = QListWidgetItem(unicode(element.toString()))
            self.content.customDirList.addItem(item)
    
    def _setPathSearch(self):
        pathSearch = QApplication.instance().settings.value(QString("path_search")).toBool()
        self.content.pathBox.setChecked(pathSearch)
    
    def _stateChanged(self, state):
        QApplication.instance().settings.setValue(QString("path_search"), QVariant(state != 0))
        self._setSearchButton()
    
    def _addDir(self):
        directory = QFileDialog.getExistingDirectory(self, self.tr("Open Directory"), "", QFileDialog.ShowDirsOnly)
        if not directory.isEmpty():
            item = QListWidgetItem(directory)
            self.content.customDirList.addItem(item)
            search_dir_list = QApplication.instance().settings.value(QString("search_dir_list")).toList()
            search_dir_list = set([d.toString() for d in search_dir_list] + [directory])
            QApplication.instance().settings.setValue(QString("search_dir_list"), QVariant(list(search_dir_list)))
            self._setSearchButton()
    
    def _removeDir(self):
        if self.content.customDirList.currentRow() != -1:
            item = self.content.customDirList.takeItem(self.content.customDirList.currentRow())
            search_dir_list = QApplication.instance().settings.value(QString("search_dir_list")).toList()
            search_dir_list = set([d.toString() for d in search_dir_list])
            search_dir_list.remove(item.text())
            QApplication.instance().settings.setValue(QString("search_dir_list"), QVariant(list(search_dir_list)))
            self._setSearchButton()
    