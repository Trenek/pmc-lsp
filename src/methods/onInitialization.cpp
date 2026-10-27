#include "LibLsp/lsp/general/initialize.h"

td_initialize::response onInitialization(td_initialize::request const& req) {
    lsTextDocumentSyncOptions sync = {
        .openClose = true,
        .change = lsTextDocumentSyncKind::Full,
    };

    td_initialize::response rsp;

    rsp.id = req.id;
    rsp.result.capabilities = {
        .textDocumentSync = std::make_pair(
            optional<lsTextDocumentSyncKind>(), 
            optional<lsTextDocumentSyncOptions>(sync)
        ),
        .hoverProvider = true,
    };

    return rsp;
}
