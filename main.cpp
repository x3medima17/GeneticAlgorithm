#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <bitset>
#include <cstring>

#include "GA.hpp"
#include <QtCore/QtCore>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QUuid>
#include <QtCore/QJsonObject>

static int Calls = 0;
static int Lost  = 0;
static int Misses = 0;
constexpr int Dimension = 2;
constexpr int Generations = 50;
constexpr int PopulationSize = 100;
constexpr size_t Crossovers = static_cast<size_t >(PopulationSize * 0.7);
constexpr double MutationRate = 0.825;
constexpr size_t acc = 60;
constexpr int CrossLoci = 32;

static_assert(CrossLoci % 2 == 0, "Even Loci");

class GriewankFactory : public  OrganismFactory, public QObject {
private:
    QUdpSocket* socket;
public:
    GriewankFactory(const std::vector<std::pair<double, double>> &Limits,
                    const double MutationRate, const size_t CrossLoci,
                    const size_t Dimension, size_t acc) :
    OrganismFactory(Limits, MutationRate, CrossLoci, Dimension, acc){
        socket = new QUdpSocket;
//        auto tmp(QUdpSocket());

        socket->bind(QHostAddress::LocalHost, 12345);

    }
    double fx(const std::vector<double>& V) const {
        double res = 0;
        for (auto item : V)
            res += item * item / 4000.0;

        double tmp = 1;
        double i = 1;
        for (const auto &item : V)
            tmp *= cos(item / sqrt(i++));
        res = res - tmp + 1;
        return res;
    }

    double compute_fitness(const Organism a) const override {
        auto V = normalize(a.DNA.get_vect());

        Calls++;
        for (size_t i = 0; i < Limits.size(); i++)
            if (V.at(i) < Limits.at(i).first || V.at(i) > Limits.at(i).second){
                std::cout<<i<<" "<<V.at(i)<<" "<<a.DNA.get_vect()[i]<<std::endl;
                return std::numeric_limits<double>::max();
            }


        QHostAddress sender;
        quint16 senderPort;
        auto uuid = QUuid::createUuid().toString();
        QUdpSocket socket;
        QJsonObject json;
        json.insert("uuid" , uuid);
        json.insert("project", "case_test");
        json.insert("action", "read");
        QJsonArray X;
        for(auto item : V)
            X.push_back(item);
        json.insert("X", X);

        QByteArray Data(QJsonDocument(json).toJson());


        socket.writeDatagram(Data, QHostAddress::LocalHost, 8989);

//        TODO
        QByteArray buff;
        buff.resize(1<<13);
        int ttl = 30000;
        while(!socket.hasPendingDatagrams() && ttl--);
        qint64 n = socket.readDatagram(buff.data(), buff.size(),
                                     &sender, &senderPort);
        if(n > 0) {
            buff = buff.mid(0,n);
//            std::cout<<n<<" "<<buff.data()<<std::endl;
            QJsonDocument itemDoc = QJsonDocument::fromJson(buff);
            QJsonObject rootObject = itemDoc.object();
            int status = rootObject.value("status").toInt();
            auto message = rootObject.value("message").toString().toStdString();
            auto uuidp = rootObject.value("uuid").toString();
//            std::cout<<status<<" "<<message<<" "<<uuidp.toStdString()<<" "<<uuid.toStdString()<<std::endl;
            assert(uuidp == uuid);
            if (uuidp == uuid && status == 0 )
                return rootObject.value("delta").toDouble();
            else
                Misses++;

        } else
            Lost++;

        double delta = fx(V);
        json["action"] = "write";
        json["delta"] = delta;
        Data = QByteArray(QJsonDocument(json).toJson());

        socket.writeDatagram(Data, QHostAddress::LocalHost, 8989);

        return delta;
    }

    ~GriewankFactory() {
        delete socket;
    }
};


int main() {
    auto p = new QUdpSocket;
    auto Limits = std::vector<std::pair<double, double>>(Dimension, {-5, 5});

//    GriewankOrganism g({10,19},10);

    auto sof = std::make_shared<GriewankFactory>
            (Limits, MutationRate, CrossLoci, Dimension, acc);
    GA ga(PopulationSize, Generations, Crossovers, sof);

    std::string stats_fname = "stats.out";
    std::ofstream stats_out(stats_fname);
    std::ofstream ga_out("ga.out");
    int gen = 0;
    auto stats = ga.run([&stats_out, &gen, &sof, &ga_out](const auto& Population){
        std::cout<<Population.front().fitness<<std::endl;
        ga_out<<gen<<std::endl;
        for (const auto &org : Population) {
            stats_out<<gen<<" ";
            auto V(sof->normalize(org.DNA.get_vect()));
            for(auto val : V)
                stats_out<<val<<" ";
            stats_out<<org.fitness<<std::endl;
        }
        gen++;
    });
    double lose_rate = (Lost*1.0/Calls*1.0)*100;
    std::cout<<"Calls:\t"<<Calls<<"\n"
             <<"Lost:\t"<<Lost<<" "<<lose_rate<<"%\n"
             <<"Misses:\t"<<Misses;
    return 0;
}