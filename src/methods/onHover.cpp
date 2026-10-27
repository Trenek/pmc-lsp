#include "LibLsp/lsp/offset_encoding.h"
#include "LibLsp/lsp/textDocument/hover.h"
#include "LibLsp/lsp/utils.h"

#include <cctype>
#include <mutex>
#include <string>

static const std::unordered_map<std::string, std::string> kDefinitions = {
    {"load", "AC ← OR"},
    {"store", "MEM[OR] ← AC"},
    {"jump", "PC ← OR"},
    {"jneg", "AC < 0 ⇒ PC ← OR"},
    {"jzero", "AC = 0 ⇒ PC ← OR"},
    {"add", "AC ← AC + OR"},
    {"sub", "AC ← AC − OR"},
    {"mult", "AC ← AC · OR"},
    {"div", "AC ← AC / OR"},
    {"and", "AC ← AC ∧ OR"},
    {"or", "AC ← AC ∨ OR"},
    {"not", "AC ← ¬OR"},
    {"cmp", "AC ← (AC = OR) ? −1 : 0"},
    {"shz", "AC ← AC · 2^OR  (shift, zeros in)"},
    {"shc", "AC ← AC rotated by OR bits"},
    {"stop", "halt"},
};

td_hover::response onHover(td_hover::request const& req, std::mutex *mutex, std::unordered_map<std::string, std::string> *documents) {
    td_hover::response rsp;
    rsp.id = req.id;
    rsp.result.contents.first.emplace();

    std::string text; {
        std::lock_guard<std::mutex> lock(*mutex);
        const auto it = documents->find(req.params.textDocument.uri.raw_uri_);
        if (it == documents->end()) return rsp;
        text = it->second;
    }

    const auto cursor = static_cast<std::size_t>(lsp::GetOffsetForPosition(req.params.position, text));
    const auto lineStart = text.rfind('\n', cursor == 0 ? 0 : cursor - 1);
    const auto from = lineStart == std::string::npos || cursor == 0 ? 0 : lineStart + 1;
    if (text.find(';', from) < cursor) return rsp;
    auto start = cursor, end = cursor;
    while (start > from && std::islower(text[start - 1])) --start;
    while (end < text.size() && std::islower(text[end])) ++end;

    const auto definition = kDefinitions.find(text.substr(start, end - start));
    if (definition == kDefinitions.end()) return rsp;
    rsp.result.contents.first.reset();
    rsp.result.contents.second = MarkupContent{"markdown", "`" + definition->second + "`"};
    rsp.result.range = lsRange(
        lsp::GetPositionForOffsetWithEncoding(start, text, lsp::OffsetEncoding::Utf16),
        lsp::GetPositionForOffsetWithEncoding(end, text, lsp::OffsetEncoding::Utf16)
    );

    return rsp;
}
