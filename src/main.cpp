#include "LibLsp/JsonRpc/stream.h"
#include "LibLsp/LspCpp.h"
#include "LibLsp/lsp/general/exit.h"

#include "methods/methods.hpp"

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

class Stdin : public lsp::stdin_istream {
    Condition<bool>& closed;

public:
    explicit Stdin(Condition<bool>& closed) : closed(closed) {}
    
    std::streamsize read_some(char* str, std::streamsize count) override {
        const auto got = lsp::stdin_istream::read_some(str, count);

        if (got <= 0 && (eof() || bad())) this->closed.notify(std::make_unique<bool>(false));

        return got;
    }
};

int main() {
    std::mutex mutex;
    std::unordered_map<std::string, std::string> documents;
    std::atomic<bool> shutdown{false};
    Condition<bool> exit_requested;

    lsp::LanguageSession server(lsp::JSONStreamStyle::Standard, 1);

    server.on(std::function(onInitialization));
    server.on(FUNCTION(td_shutdown)(std::bind(onShutdown, std::placeholders::_1, &shutdown)));
    server.on([&](Notify_Exit::notify&) { exit_requested.notify(std::make_unique<bool>(true)); });
    server.on(NFUNCTION(Notify_TextDocumentDidOpen)(std::bind(onNotiyTextDocumentDidOpen, std::placeholders::_1, &mutex, &documents)));
    server.on(NFUNCTION(Notify_TextDocumentDidChange)(std::bind(onNotiyTextDocumentDidChange, std::placeholders::_1, &mutex, &documents)));
    server.on(NFUNCTION(Notify_TextDocumentDidClose)(std::bind(onNotiyTextDocumentDidClose, std::placeholders::_1, &mutex, &documents)));
    server.on(FUNCTION(td_hover)(std::bind(onHover, std::placeholders::_1, &mutex, &documents)));

    server.start(std::make_shared<Stdin>(exit_requested), lsp::make_stdout_stream());
    exit_requested.wait();
    server.stop();

    return !shutdown;
}
