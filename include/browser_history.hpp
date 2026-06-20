#pragma once
#include <string>
#include <stack>
#include <stdexcept>

// Models browser navigation history exactly the way real browsers do it:
// one stack for "back" pages, one stack for "forward" pages, and a
// pointer to the current page.
//
//   visit(url)  -> push current page onto backStack, go to url,
//                  clear forwardStack (new navigation erases old "forward" path)
//   back()      -> push current page onto forwardStack, pop from backStack
//   forward()   -> push current page onto backStack, pop from forwardStack
class BrowserHistory {
public:
    explicit BrowserHistory(const std::string& homepage) : currentUrl(homepage) {}

    void visit(const std::string& url) {
        backStack.push(currentUrl);
        currentUrl = url;
        // Visiting a new page invalidates whatever "forward" history existed.
        while (!forwardStack.empty()) forwardStack.pop();
    }

    std::string back() {
        if (backStack.empty()) {
            throw std::runtime_error("Already at the oldest page in history");
        }
        forwardStack.push(currentUrl);
        currentUrl = backStack.top();
        backStack.pop();
        return currentUrl;
    }

    std::string forward() {
        if (forwardStack.empty()) {
            throw std::runtime_error("No forward history available");
        }
        backStack.push(currentUrl);
        currentUrl = forwardStack.top();
        forwardStack.pop();
        return currentUrl;
    }

    const std::string& current() const { return currentUrl; }
    bool canGoBack() const { return !backStack.empty(); }
    bool canGoForward() const { return !forwardStack.empty(); }
    size_t backDepth() const { return backStack.size(); }
    size_t forwardDepth() const { return forwardStack.size(); }

private:
    std::string currentUrl;
    std::stack<std::string> backStack;
    std::stack<std::string> forwardStack;
};
