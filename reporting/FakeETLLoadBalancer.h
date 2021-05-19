#ifndef FAKEETLLOADBALANCER_H_INCLUDED
#define FAKEETLLOADBALANCER_H_INCLUDED
#include "org/xrpl/rpc/v1/xrp_ledger.grpc.pb.h"
#include <map>
#include <reporting/DBHelpers.h>
#include <reporting/ETLHelpers.h>
#include <reporting/ETLLoadBalancer.h>
#include <string>
#include <vector>
class FakeETLLoadBalancer : public ETLLoadBalancer
{
    std::vector<org::xrpl::rpc::v1::GetLedgerDataResponse> getLedgerData_;
    std::map<uint32_t, org::xrpl::rpc::v1::GetLedgerResponse> getLedgerMap_;

public:
    FakeETLLoadBalancer(NetworkValidatedLedgers& nwvl);

    ~FakeETLLoadBalancer()
    {
    }

    void
    loadInitialLedger(
        uint32_t sequence,
        std::function<void(org::xrpl::rpc::v1::RawLedgerObject&)> f) override;

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
    fetchLedger(uint32_t ledgerSequence, bool getObjects) override;

    void
    start() override
    {
    }

    void
    stop() override
    {
    }
};
#endif
