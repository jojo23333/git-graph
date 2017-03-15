#ifndef QGRAPH_H
#define QGRAPH_H

#include <algorithms/FastUnfolding.h>
#include <algorithms/GitGraph.h>
#include <algorithms/GN.h>
//#include <algorithms/readgml.h>
#include <algorithms/PageRank.h>
#include <stdio.h>
#include <QDebug>
#include <QFile>

#include <QObject>
#include <QString>

class QGraph : public QObject{
    Q_OBJECT
    Q_PROPERTY(int node_amount READ node_amount)
    Q_PROPERTY(int edge_amount READ edge_amount)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
public:
    void setUrl(const QString &file){
        qDebug()<<"hello";
        string filepath=file.toStdString();
        bool flag = init_graph(filepath);
        m_url = QString::fromStdString(filepath);
        if(flag){
            emit urlChanged();
        }
        else
            emit urlInvalid();
    }
    int node_amount() const{
        return m_node_amount;
    }
    int edge_amount() const{
        return m_edge_amount;
    }
    QString url() const{
        return m_url;
    }

signals:
    void urlChanged();
    void urlInvalid();
private:
    gitgraph *g;
    int m_node_amount,m_edge_amount;
    QString m_url;
    bool init_graph(string filepath)
    {
        NETWORK net;
        FILE *fp=NULL;
        QFile f("C:/Users/77936/Pictures/karate.gml");
        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<<f.handle();
        qDebug()<<f.symLinkTarget();
        //qDebug()<<filepath.c_str();
        fp = fdopen(f.handle(),"r");
        if(!fp) return false;
        read_network(&net,fp);
        qDebug()<<"ok";
        g = new gitgraph(&net);
        return true;
    }
};

#endif // QGRAPH_H
