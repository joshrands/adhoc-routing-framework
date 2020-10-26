#ifndef REMMODEL_H
#define REMMODEL_H
/*********************************
 * rem_model.h
 *
 * C++ implementation of REgression-based network Monitoring prediction models  
 * Formerly known as Model.h***
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

#define RSS_OUT_OF_RANGE        -90
// Max rss data points to hold
#define RSS_DATA_LIMIT			100

#include "rem_parameters.h"
#include "rem_defines.h"
#include "rem_packet.h"
#include "routing_protocol.h"

#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

/*
PredictionModel: Abstract class representing all prediction models
*/
class PredictionModel
{
public:
    double ERROR_MAX = 20; // percentage error max
    int MAX_DIFF_COUNT = 4; // number of bad data point in a row
    double MAX_THRESH_DIFF = 30; // difference that data can be to be accepted, was 16
    uint16_t INIT_COUNT = 3; // number of data values needed to build a model
    double NUM_DEVIATIONS = 4; // number of standard deviations to fall within threshold
    double MIN_DEVIATION = MAX_THRESH_DIFF / 5;

    int UPDATE_FREQUENCY  = 1; // update model every x seconds
    uint16_t DEFAULT_TTL  = 5; // default ttl is x seconds
    int PACKET_SIZE       = 19; // 13 bytes as of now
    int HOP_COUNT         = 2;

    // set depending on battery or rss
    uint8_t MODEL_TYPE;

    virtual void initialize() = 0;

    /**** MODEL FUNCTIONS ****/
    void addDataPoint(double value, double time); // add a data point to the current model and call fit model
    void calculateDeviations(vector<double> times, vector<double> values); // calculate sigma for the given data set
    bool withinExpectedValueRange(double time, double value); // check if the calculated value is a valid prediction
    bool compareToPreviousModel();
    void adaptModel(); // adapt the current model for the latest measured data in window
    // TODO: Function for cleaning data and providing good points in window

    // ABSTRACT
    virtual double getDataPoint(double time) = 0; // abstract function for getting predicted data point
    virtual void fitModel() = 0; // fit the current model to data points
    virtual void performRegression(vector<double> times, vector<double> values, vector<double> wAvgs) = 0; // perform a regression on the data provided
    /*************************/

    int getDataCount() { return mvData.size(); }
    int getWindowSize() { return windowSize; }

    ModelState getState() { return state; }
    void setState(ModelState state) { this->state = state; }

    // NS3-TODO: ADD THIS IN HELPER  void setParentNode(Ptr<Node> parent);

    ModelParameters modelParameters;
    IP_ADDR ownerIp;

    // broadcast socket for this model to communicate on
    /* NS3-TODO: ADD THESE IN HELPER 
    Ptr<Socket> socket;
    Ptr<Node> parentNode;
    */

    // only for RSS model
    IP_ADDR pairIp; // ip of pair node

    REMModelPacket createREMModelPacket();

    bool needsToBeBroadcasted;

protected:
    double timeToLive; // ttl
    ModelState state;

    vector<double> mvData;
    vector<double> mvTimes;
    vector<double> mvWeightedAverages;

    // consecutive bad data counter
    int difCount = 0; // initialize to 0

    // previous deviation for trend analysis
    double lastDeviation = 0;

    // model parameters
    ModelParameters oldModel;

    // weighted average parameter
    double alpha = 0.9; // consider changing this to 0.7-0.9

    // dynamic window size
    int windowSize;
};

/*
BatteryModel: Prediction model for local parameter battery level of a node
*/
class BatteryModel : public PredictionModel
{
public:
    BatteryModel(); // default constructor

    // NS3-TODO: HOW GET BATTERY?  Ptr<EnergySource> battery;

    void initialize();

    double getDataPoint(double time);

    void setAlphaTimeout(double val) { this->alphaTimeout = val; }
    double getAlphaTimeout() { return this->alphaTimeout; }

private:
    double alphaTimeout;

    void fitModel();
    void performRegression(vector<double> times, vector<double> values, vector<double> wAvgs); // perform a regression on the data provided
};

/*
RssModel: Prediction model for pair-wise parameter RSS of a node in a NetworkMonitor
*/
class RssModel : public PredictionModel
{
public:
    RssModel(); // default constructor

    void initialize();

    double getDataPoint(double time);

private:

    void fitModel();
    void performRegression(vector<double> times, vector<double> values, vector<double> wAvgs); // perform a regression on the data provided
};

#endif
