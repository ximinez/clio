#include "org/xrpl/rpc/v1/xrp_ledger.grpc.pb.h"
#include <google/protobuf/util/json_util.h>
#include <map>
#include <reporting/FakeETLLoadBalancer.h>
#include <reporting/data/FakeETLData.h>
#include <string>
#include <vector>

FakeETLLoadBalancer::FakeETLLoadBalancer(NetworkValidatedLedgers& nwvl)
{
    for (auto& diff : diffsProtobuf)
    {
        org::xrpl::rpc::v1::GetLedgerResponse getLedgerRes;
        google::protobuf::util::JsonStringToMessage(diff, &getLedgerRes);
        auto lgrInfo =
            deserializeHeader(ripple::makeSlice(getLedgerRes.ledger_header()));
        getLedgerMap_[lgrInfo.seq] = getLedgerRes;
        nwvl.push(lgrInfo.seq);
    }
    for (auto& res : firstLedgerProtobufs)
    {
        org::xrpl::rpc::v1::GetLedgerDataResponse getLedgerDataRes;
        google::protobuf::util::JsonStringToMessage(res, &getLedgerDataRes);
        getLedgerData_.push_back(getLedgerDataRes);
    }
}
void
FakeETLLoadBalancer::loadInitialLedger(
    uint32_t sequence,
    std::function<void(org::xrpl::rpc::v1::RawLedgerObject&)> f)
{
    for (auto& res : getLedgerData_)
    {
        for (auto& obj : *res.mutable_ledger_objects()->mutable_objects())
        {
            f(obj);
        }
    }
}

/// Fetch data for a specific ledger. This function will continuously try
/// to fetch data for the specified ledger until the fetch succeeds, the
/// ledger is found in the database, or the server is shutting down.
/// @param ledgerSequence sequence of ledger to fetch data for
/// @param getObjects if true, fetch diff between specified ledger and
/// previous
/// @return the extracted data, if extraction was successful. If the ledger
/// was found in the database or the server is shutting down, the optional
/// will be empty
std::optional<org::xrpl::rpc::v1::GetLedgerResponse>
FakeETLLoadBalancer::fetchLedger(uint32_t ledgerSequence, bool getObjects)
{
    return getLedgerMap_[ledgerSequence];
}

