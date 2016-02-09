#include "MapInterface.h"


#ifndef PENDER_H
#define PENDER_H

class MaxMin{
public:
    uint max, min;
    MaxMin(){MaxMin(0,0);}
    MaxMin(uint mi, uint mx){min=mi;max=mx;}
    void updateMaxMin(uint x1, uint x2){
        min = (x1<min)?x1:min;
        max = (x2>max)?x2:max;
    }
};

class IsoHolder{
public:
    MaxMin maxmin;
    QMap<QString, MaxMin> extras;
    IsoHolder(QMap<QString, MaxMin> extras, MaxMin maxmin){
        this->extras = extras;
        this->maxmin = maxmin;
    }
};

class GeneHolder{
public:
    MaxMin maxmin;
    QMap<QString, IsoHolder*> isos;
    GeneHolder(QMap<QString, IsoHolder*> genesAndIsos, MaxMin maxmin){
        this->isos = genesAndIsos;
        this->maxmin = maxmin;
    }
};


class Pender : MapInterface
{
public:
    Pender(QString &map, QString &colf, QString &rejects, QString &outf,
           bool &append, bool &skipbad, bool &keepints, bool &force_processing)
    {
        mapfile = map;
        colfile = colf;
        outfile = outf;

        appendmultiple = append;
        skipbadscore = skipbad;
        keepall = keepints;
        forceprocs = force_processing;

        if (!forceprocs){
            bool processedInput  = alreadyProcessed(colf);
            bool processedOutput = alreadyProcessed(outf);


            if (processedInput){
                cerr << "Gene headers present in input. Skipping" << endl;
                exit(1);
            }
            if (processedOutput){
                cerr << "Gene headers present in output. Skipping." << endl;
                exit(2);
            }
        }

        // Either exitted due to previous processing, or go ahead given.

        //Open Channel
        rej.open(rejects.toUtf8().data());
        out.open(outf.toUtf8().data());

        populateMap();
        appendToVCF();
    }


    ~Pender(){
        rej.close();
        out.close();
    }

    //chrom-->Genesos --> Gene --> Exon
    QMap<QString, QMap<QString, GeneHolder*> > chromemap;
    //chrom-->Gene name --> maxmin, and geneSo


    void appendToVCF();
    void populateMap();
    bool alreadyProcessed(QString filename);
};

#endif // PENDER_H
