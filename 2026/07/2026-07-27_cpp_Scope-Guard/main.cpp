#include <iostream>
#include <functional>
#include <utility>

class ScopeGuard {
public:
    template <typename Callable,
              typename = std::enable_if_t<std::is_invocable_v<Callable>>>
    explicit ScopeGuard(Callable&& callable)
        : callable_(std::forward<Callable>(callable)), active_(true) {}

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& other) noexcept
        : callable_(std::move(other.callable_)), active_(other.active_) {
        other.active_ = false;
    }

    ScopeGuard& operator=(ScopeGuard&& other) noexcept {
        if (active_) {
            callable_();
        }
        return *this;
    }

    void dismiss() noexcept {
        active_ = false;
    }

    ~ScopeGuard() {
        if (active_ && callable_) {
            callable_();
        }
    }

private:
    std::function<void()> callable_;
    bool active_;
};

int main (){

    std::cout << "Start\n";

    {
        ScopeGuard guard([]() {
            std::cout << "ScopeGuard executed\n";
        });

        std::cout << "Inside scope\n";
    }
    std::cout << "End\n";
    return 0;
}