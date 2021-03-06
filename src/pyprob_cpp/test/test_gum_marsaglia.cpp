#include <pyprob_cpp.h>

// Gaussian with unkown mean (Marsaglia)
// http://www.robots.ox.ac.uk/~fwood/assets/pdf/Wood-AISTATS-2014.pdf

double marsaglia(double mean, double stddev)
{
  auto uniform = pyprob_cpp::distributions::Uniform(-1, 1);
  auto s = 1.0, x = 0.0, y = 0.0;
  while (s >= 1)
  {
    x = pyprob_cpp::sample(uniform)(0);
    y = pyprob_cpp::sample(uniform)(0);
    s = x*x + y*y;
  }
  return mean + stddev * (x * std::sqrt(-2 * std::log(s) / s));
}


xt::xarray<double> forward()
{
  auto prior_mean = 1;
  auto prior_stddev = std::sqrt(5);
  auto likelihood_stddev = std::sqrt(2);

  auto mu = marsaglia(prior_mean, prior_stddev);

  auto likelihood = pyprob_cpp::distributions::Normal(mu, likelihood_stddev);
  pyprob_cpp::observe(likelihood, "obs0");
  pyprob_cpp::observe(likelihood, "obs1");

  return xt::xarray<double> {mu};
}


int main(int argc, char *argv[])
{
  auto serverAddress = (argc > 1) ? argv[1] : "tcp://*:5555";
  pyprob_cpp::Model model = pyprob_cpp::Model(forward, "Gaussian with unkown mean (Marsaglia) C++");
  model.startServer(serverAddress);
  return 0;
}
