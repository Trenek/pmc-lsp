#include "LibLsp/LspCpp.h"
#include "LibLsp/lsp/general/shutdown.h"

#include <atomic>
#include <string>

td_shutdown::response onShutdown(td_shutdown::request const& req, std::atomic<bool> *shutdown) {
    td_shutdown::response rsp;

    rsp.id = req.id;
    rsp.result.emplace().SetJsonString("null", lsp::Any::kNullType);

    *shutdown = true;

    return rsp;
}
