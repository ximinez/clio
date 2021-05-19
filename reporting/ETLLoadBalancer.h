#ifndef ETLLOADBALANCER_H_INCLUDED
#define ETLLOADBALANCER_H_INCLUDED

#include "org/xrpl/rpc/v1/xrp_ledger.grpc.pb.h"
#include <optional>
/// This class is used to manage connections to transaction processing processes
/// This class spawns a listener for each etl source, which listens to messages
/// on the ledgers stream (to keep track of which ledgers have been validated by
/// the network, and the range of ledgers each etl source has). This class also
/// allows requests for ledger data to be load balanced across all possible etl
/// sources.
class ETLLoadBalancer
{
public:
    virtual void
    loadInitialLedger(
        uint32_t sequence,
        std::function<void(org::xrpl::rpc::v1::RawLedgerObject&)> f) = 0;

    /// Fetch data for a specific ledger. This function will continuously try
    /// to fetch data for the specified ledger until the fetch succeeds, the
    /// ledger is found in the database, or the server is shutting down.
    /// @param ledgerSequence sequence of ledger to fetch data for
    /// @param getObjects if true, fetch diff between specified ledger and
    /// previous
    /// @return the extracted data, if extraction was successful. If the ledger
    /// was found in the database or the server is shutting down, the optional
    /// will be empty
    virtual std::optional<org::xrpl::rpc::v1::GetLedgerResponse>
    fetchLedger(uint32_t ledgerSequence, bool getObjects) = 0;

    /// Setup all of the ETL sources and subscribe to the necessary streams
    virtual void
    start() = 0;

    virtual void
    stop() = 0;
    virtual ~ETLLoadBalancer()
    {
    }
};

#endif
