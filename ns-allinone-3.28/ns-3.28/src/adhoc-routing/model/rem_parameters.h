/*********************************
 * rem_parameters.h
 *
 * C++ implementation of REgression-based network Monitoring prediction models 
 * 
 * Author: Josh Rands
 * Date: 10/28/2019
 ********************************/

enum ModelType
{
  BATTERY = 1,
  RSS = 2
};

enum ModelState
{
  INIT,
  STABLE,
  UNSTABLE,
  BAD,
  OLD
};

class ModelParameters
{
public:
    ModelParameters() { }; // default constructor
    ModelParameters(float mu, float beta, float sigma) { this->mu = mu; this->beta = beta; this->sigma = sigma; };

    float mu;
    float beta;
    float sigma; // standard deviation

    ModelType type; // battery, rss, other
    int timeToLive; // time to live in seconds
    int ownerId;
    int pairId; // if rss data
    int hopsRemaining;

    int UPDATE_FREQUENCY = 1;
};
