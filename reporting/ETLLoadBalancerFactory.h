#ifndef ETLLOADBALANCERFACTORY_H_INCLUDED
#define ETLLOADBALANCERFACTORY_H_INCLUDED
#include <reporting/ETLLoadBalancer.h>
#include <reporting/ETLSource.h>
#include <reporting/FakeETLLoadBalancer.h>
std::unique_ptr<ETLLoadBalancer>
makeETLLoadBalancer(
    boost::json::array const& config,
    NetworkValidatedLedgers& nwvl,
    boost::asio::io_context& ioContext)
{
    if (config.size() > 0 && config[0].is_string() &&
        std::string{config[0].as_string().c_str()} == "fake")
    {
        auto etl = std::make_unique<FakeETLLoadBalancer>(nwvl);
        return std::move(etl);
    }
    else
    {
        auto etl =
            std::make_unique<ETLLoadBalancerImpl>(config, nwvl, ioContext);
        return std::move(etl);
    }
}
#endif
