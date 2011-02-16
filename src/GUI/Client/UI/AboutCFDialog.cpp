// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "Common/CF.hpp"

#include "GUI/Client/UI/AboutCFDialog.hpp"

using namespace CF::GUI::ClientUI;

AboutCFDialog::AboutCFDialog(QWidget * parent)
  : QDialog(parent)
{
  this->setWindowTitle("About CF");

  m_mainLayout = new QVBoxLayout(this);
  m_infoLayout = new QFormLayout();

  m_btOK = new QPushButton("OK");

  m_labQwt = new QLabel("COOLFluiD client is based in part on the \n"
                        "work of the Qwt project (http://qwt.sf.net).", this);

  m_infoList << new CFInfo("CF version:", CF_VERSION_STR, m_infoLayout);
  m_infoList << new CFInfo("Kernel version:", CF_KERNEL_VERSION_STR, m_infoLayout);
  m_infoList << new CFInfo("Build operating system:", QString("%1 [%2bits]").arg(CF_OS_LONGNAME).arg(CF_OS_BITS), m_infoLayout);
  m_infoList << new CFInfo("Build processor:", CF_BUILD_PROCESSOR, m_infoLayout);
  m_infoList << new CFInfo("Qt version:", QT_VERSION_STR, m_infoLayout);


  m_mainLayout->addLayout(m_infoLayout);
  m_mainLayout->addWidget(m_labQwt);
  m_mainLayout->addWidget(m_btOK);

  connect(m_btOK, SIGNAL(clicked()), this, SLOT(accept()));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AboutCFDialog::~AboutCFDialog()
{
  delete m_btOK;
  delete m_labQwt;
  delete m_infoLayout;
  delete m_mainLayout;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AboutCFDialog::CFInfo::CFInfo(const QString & name, const QString & value,
                              QFormLayout * parent)
{
  this->labName = new QLabel(name);
  this->labValue = new QLabel(value);

  parent->addRow(this->labName, this->labValue);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AboutCFDialog::CFInfo::~CFInfo()
{
  delete this->labName;
  delete this->labValue;
}
