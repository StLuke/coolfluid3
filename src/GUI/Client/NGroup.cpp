#include <QtCore>
#include <QtGui>

#include "GUI/Client/NGroup.hpp"

using namespace CF::GUI::Client;

NGroup::NGroup(const QString & name) :
    CNode(name, "CGroup", GROUP_NODE)
{

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QIcon NGroup::getIcon() const
{
  return QFileIconProvider().icon(QFileIconProvider::Folder);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QString NGroup::getToolTip() const
{
  return this->getComponentType();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void NGroup::getOptions(QList<NodeOption> & params) const
{
  params = m_options;
}

