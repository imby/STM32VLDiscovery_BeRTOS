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

import BFolderPage
import BVersionPage
import BCpuPage
import BToolchainPage
import BModulePage
import BOutputPage

class BWizard(QWizard):
    
    def __init__(self):
        QWizard.__init__(self)
        # TODO: choose the right minimum size
        self.setMinimumSize(1000, 500)
        self.setWindowTitle(self.tr("Create a BeRTOS project"))
        self._addPages()
        self._connectSignals()
    
    def _addPages(self):
        self.addPage(BFolderPage.BFolderPage())
        self.addPage(BVersionPage.BVersionPage())
        self.addPage(BCpuPage.BCpuPage())
        self.addPage(BToolchainPage.BToolchainPage())
        self.addPage(BModulePage.BModulePage())
        self.addPage(BOutputPage.BOutputPage())
    
    def _connectSignals(self):
        self.connect(self, SIGNAL("currentIdChanged(int)"), self._pageChanged)
    
    def _pageChanged(self, pageId):
        prevPage = self.page(pageId - 1)
        if prevPage is not None:
            prevPage.saveData()
        page = self.page(pageId)
        if page is not None:
            page.reloadData()