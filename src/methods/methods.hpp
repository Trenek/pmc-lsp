#include "LibLsp/LspCpp.h"
#include "LibLsp/lsp/general/initialize.h"
#include "LibLsp/lsp/general/shutdown.h"
#include "LibLsp/lsp/textDocument/did_change.h"
#include "LibLsp/lsp/textDocument/did_close.h"
#include "LibLsp/lsp/textDocument/did_open.h"
#include "LibLsp/lsp/textDocument/hover.h"

td_initialize::response onInitialization(td_initialize::request const& req);
td_shutdown::response onShutdown(td_shutdown::request const& req, std::atomic<bool> *shutdown);
td_hover::response onHover(td_hover::request const& req, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents);

void onNotiyTextDocumentDidOpen(Notify_TextDocumentDidOpen::notify& n, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents);
void onNotiyTextDocumentDidChange(Notify_TextDocumentDidChange::notify& n, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents);
void onNotiyTextDocumentDidClose(Notify_TextDocumentDidClose::notify& n, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents);

#define FUNCTION(T) std::function<T::response(const T::request&)>
#define NFUNCTION(T) std::function<void(T::notify&)>
