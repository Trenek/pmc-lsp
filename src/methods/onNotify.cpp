#include "LibLsp/lsp/textDocument/did_change.h"
#include "LibLsp/lsp/textDocument/did_close.h"
#include "LibLsp/lsp/textDocument/did_open.h"

#include <mutex>
#include <string>
#include <unordered_map>

void onNotiyTextDocumentDidOpen(Notify_TextDocumentDidOpen::notify& n, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents) {
    std::lock_guard<std::mutex> lock(*mutex);

    (*documents)[n.params.textDocument.uri.raw_uri_] = n.params.textDocument.text;
}

void onNotiyTextDocumentDidChange(Notify_TextDocumentDidChange::notify& n, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents) {
    std::lock_guard<std::mutex> lock(*mutex);

    if (!n.params.contentChanges.empty()) {
        (*documents)[n.params.textDocument.uri.raw_uri_] = n.params.contentChanges.back().text;
    }
}

void onNotiyTextDocumentDidClose(Notify_TextDocumentDidClose::notify& n, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents) {
    std::lock_guard<std::mutex> lock(*mutex);

    documents->erase(n.params.textDocument.uri.raw_uri_);
}
