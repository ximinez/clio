#ifndef REPORTING_RPC_H_INCLUDED
#define REPORTING_RPC_H_INCLUDED

#include <optional>
#include <string>
#include <variant>
#include <boost/json.hpp>
#include <ripple/protocol/ErrorCodes.h>
#include <backend/BackendInterface.h>
/*
 * This file contains various classes necessary for executing RPC handlers.
 * Context gives the handlers access to various other parts of the application
 * Status is used to report errors.
 * And lastly, there are various functions for making Contexts, Statuses and
 * serializing Status to JSON.
 * This file is meant to contain any class or function that code outside of the
 * rpc folder needs to use. For helper functions or classes used within the rpc
 * folder, use RPCHelpers.h.
 */

class WsBase;
class SubscriptionManager;
class ETLLoadBalancer;

namespace RPC
{

struct Context
{
    std::string method;
    std::uint32_t version;
    boost::json::object const& params;
    std::shared_ptr<BackendInterface> const& backend;
    std::shared_ptr<SubscriptionManager> const& subscriptions;
    std::shared_ptr<ETLLoadBalancer> const& balancer;
    std::shared_ptr<WsBase> session;
    Backend::LedgerRange const& range;

    Context(
        std::string const& command_,
        std::uint32_t version_,
        boost::json::object const& params_,
        std::shared_ptr<BackendInterface> const& backend_,
        std::shared_ptr<SubscriptionManager> const& subscriptions_,
        std::shared_ptr<ETLLoadBalancer> const& balancer_,
        std::shared_ptr<WsBase> const& session_,
        Backend::LedgerRange const& range_)
    : method(command_)
    , version(version_)
    , params(params_)
    , backend(backend_)
    , subscriptions(subscriptions_)
    , balancer(balancer_)
    , session(session_)
    , range(range_)
    {}
};
using Error = ripple::error_code_i;

struct Status
{
    Error error = Error::rpcSUCCESS;
    std::string message = "";

    Status() {};

    Status(Error error_) : error(error_) {};

    Status(Error error_, std::string message_)
    : error(error_)
    , message(message_) 
    {}

    /** Returns true if the Status is *not* OK. */
    operator bool() const
    {
        return error != Error::rpcSUCCESS;
    }
};

static Status OK;

using Result = std::variant<Status, boost::json::object>;

void
inject_error(Error err, boost::json::object& json);

void
inject_error(Error err, std::string const& message, boost::json::object& json);

boost::json::object
make_error(Error err);

boost::json::object
make_error(Error err, std::string const& message);


std::optional<Context>
make_WsContext(
    boost::json::object const& request,
    std::shared_ptr<BackendInterface> const& backend,
    std::shared_ptr<SubscriptionManager> const& subscriptions,
    std::shared_ptr<ETLLoadBalancer> const& balancer,
    std::shared_ptr<WsBase> const& session,
    Backend::LedgerRange const& range);

std::optional<Context>
make_HttpContext(
    boost::json::object const& request,
    std::shared_ptr<BackendInterface> const& backend,
    std::shared_ptr<SubscriptionManager> const& subscriptions,
    std::shared_ptr<ETLLoadBalancer> const& balancer,
    Backend::LedgerRange const& range);

Result
buildResponse(Context const& ctx);
    
} // namespace RPC

#endif //REPORTING_RPC_H_INCLUDED
