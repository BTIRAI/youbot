#ifndef XMLPARSERS_HPP
#define XMLPARSERS_HPP

#include <QtXml>
#include <QList>
#include <node_editor/Node>
#include <node_editor/NodeData>
#include <node_editor/FlowScene>

void ParseBehaviorTreeXML(const QDomElement &xml_root , QtNodes::FlowScene* scene, QtNodes::Node& qt_root);

void ParseLooseNodesXML(const QDomElement &xml_root, QtNodes::FlowScene* scene, QtNodes::Node& qt_root );

class StateUpdateXmlHandler :public QXmlDefaultHandler, public QObject
{

public:
    StateUpdateXmlHandler(QtNodes::FlowScene* scene, QtNodes::Node* root );

    ~StateUpdateXmlHandler() { }

    bool startElement(const QString & namespaceURI, const QString & localName,
                      const QString & qName, const QXmlAttributes & atts );

    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);

private:
    QtNodes::FlowScene* _scene;
    QList<std::pair<QtNodes::Node*, int> > _nested_nodes;
    QtNodes::Node* _root;
};



#endif // XMLPARSERS_HPP
