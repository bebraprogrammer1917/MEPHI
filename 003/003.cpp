#include <iostream>
#include <chrono>
#include <string>
#include <stdexcept>
#include <random>
#include <format>

namespace quantities {

    struct Volt   { double value; };
    struct Ampere { double value; };
    struct Ohm    { double value; };
    struct Joule  { double value; };
    struct Watt   { double value; };
    struct Second { double value; };

    constexpr Volt   operator"" _V(long double v)   { return Volt{static_cast<double>(v)}; }
    constexpr Ampere operator"" _A(long double a)   { return Ampere{static_cast<double>(a)}; }
    constexpr Ampere operator"" _mA(long double a)  { return Ampere{static_cast<double>(a) / 1000.0}; }
    constexpr Ohm    operator"" _Ohm(long double o) { return Ohm{static_cast<double>(o)}; }
    constexpr Joule  operator"" _J(long double j)   { return Joule{static_cast<double>(j)}; }
    constexpr Watt   operator"" _W(long double w)   { return Watt{static_cast<double>(w)}; }
    constexpr Second operator"" _s(long double s)   { return Second{static_cast<double>(s)}; }

    Volt operator*(const Ampere& i, const Ohm& r) {
        return Volt{ i.value * r.value };
    }

    Ampere operator/(const Volt& u, const Ohm& r) {
        return Ampere{ u.value / r.value };
    }

    Ohm operator/(const Volt& u, const Ampere& i) {
        return Ohm{ u.value / i.value };
    }

    Watt operator*(const Volt& u, const Ampere& i) {
        return Watt{ u.value * i.value };
    }

    Joule operator*(const Watt& p, const Second& t) {
        return Joule{ p.value * t.value };
    }

}

template <typename T>
struct Measurement {
    std::chrono::system_clock::time_point time;
    T value;
    double error;
};

template <typename T>
struct std::formatter<Measurement<T>> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Measurement<T>& m, std::format_context& ctx) const {
        auto t = std::chrono::system_clock::to_time_t(m.time);
        std::string time_str = std::ctime(&t);
        if (!time_str.empty() && time_str.back() == '\n') time_str.pop_back();
        return std::format_to(ctx.out(), "[{}] value = {}, error = {}",
                              time_str, m.value.value, m.error);
    }
};

namespace devices {

    class Voltmeter {
    private:
        double min_range;
        double max_range;
        double accuracy;
        std::string id;

    public:
        Voltmeter(double min, double max, double acc, std::string identifier)
            : min_range(min), max_range(max), accuracy(acc), id(std::move(identifier))
        {
            if (min_range > max_range) {
                throw std::invalid_argument("min > max");
            }
            if (accuracy <= 0) {
                throw std::invalid_argument("accuracy must be > 0");
            }
        }

        double get_min()  const { return min_range; }
        double get_max()  const { return max_range; }
        double get_acc()  const { return accuracy; }
        std::string get_id() const { return id; }
 
        Measurement<quantities::Volt> measure() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(min_range, max_range);

        double val = dist(gen);
        double err = (max_range - min_range) * accuracy / 100.0;

        return Measurement<quantities::Volt>{
            std::chrono::system_clock::now(),
            quantities::Volt{val},
            err
            };
        }
    };
}

void demo_quantities() {
    using namespace quantities;
    std::cout << "=== 1. Quantities and suffixes ===\n";

    auto v  = 220.0_V;
    auto a  = 2.0_A;
    auto r  = 50.0_Ohm;
    auto ma = 500.0_mA;

    std::cout << "v  = " << v.value  << " V\n";
    std::cout << "a  = " << a.value  << " A\n";
    std::cout << "r  = " << r.value  << " Ohm\n";
    std::cout << "ma = " << ma.value << " A\n";
}

void demo_operators() {
    using namespace quantities;
    std::cout << "\n=== 2. Ohm's law (operators) ===\n";

    auto voltage    = 2.0_A   * 50.0_Ohm;
    auto current    = 220.0_V / 50.0_Ohm;
    auto resistance = 220.0_V / 2.0_A;
    auto power      = 220.0_V * 2.0_A;
    auto energy     = power   * 5.0_s;

    std::cout << "U = " << voltage.value    << " V\n";
    std::cout << "I = " << current.value    << " A\n";
    std::cout << "R = " << resistance.value << " Ohm\n";
    std::cout << "P = " << power.value      << " W\n";
    std::cout << "A = " << energy.value     << " J\n";
}

void demo_measurement() {
    using namespace quantities;
    std::cout << "\n=== 3. Measurement<T> ===\n";

    Measurement<quantities::Volt> m;
    m.time  = std::chrono::system_clock::now();
    m.value = 220.0_V;
    m.error = 1.5;

    std::cout << "Value = " << m.value.value << " V\n";
    std::cout << "Error = " << m.error       << "\n";
}

void demo_voltmeter() {
    std::cout << "\n=== 4. Voltmeter (class) ===\n";

    devices::Voltmeter vm(0.0, 250.0, 1.0, "V-001");
    std::cout << "ID       : " << vm.get_id()  << "\n";
    std::cout << "Range    : " << vm.get_min() << " .. " << vm.get_max() << " V\n";
    std::cout << "Accuracy : " << vm.get_acc() << "%\n";

    try {
        devices::Voltmeter bad(300.0, 100.0, 1.0, "BAD");
    } catch (const std::invalid_argument& e) {
        std::cout << "Invariant check OK: " << e.what() << "\n";
    }
}

void demo_format() {
    std::cout << "\n=== 5. measure() + std::format ===\n";

    devices::Voltmeter vm(0.0, 250.0, 1.0, "V-002");
    auto result = vm.measure();

    std::cout << std::format("{}\n", result);
}

int main() {
    demo_quantities();
    demo_operators();
    demo_measurement();
    demo_voltmeter();
    demo_format();
    return 0;
}
