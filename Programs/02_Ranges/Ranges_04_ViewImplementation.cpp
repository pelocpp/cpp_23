// ===========================================================================
// Ranges_Ex_04_ViewImplementation.cpp
// ===========================================================================

#include <algorithm>
#include <ranges>
#include <string>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace Cpp20RangesViewImplementationExample_01
{
    // https://github.com/andreasfertig/programming-with-cpp20
    // Sehr ähnlich zu 
    // https://mariusbancila.ro/blog/2020/06/06/a-custom-cpp20-range-view/

    // namespace rv = std::ranges::views;
    
    template<std::ranges::view R>  // #A Using ranges::view  concept
    class custom_take_view
        : public std::ranges::view_interface<custom_take_view<R>> {
        // #B Necessary data members
        R                                  base_{};
        std::ranges::range_difference_t<R> count_{};

    public:
        // #C Default constructible
        custom_take_view() = default;

        // #D Constructor for range and count
        constexpr custom_take_view(
            R                                  base,
            std::ranges::range_difference_t<R> count)
            : base_{ std::move(base) }
            , count_{ count }
        {}

        // #E view_interface members
        constexpr R base() const& { return base_; }
        constexpr R base()&& { return std::move(base_); }

        // #F Actual begin and end
        constexpr auto begin() { return std::ranges::begin(base_); }
        constexpr auto end()
        {
            return std::ranges::next(std::ranges::begin(base_), count_);
        }
    };

    template<std::ranges::range R>  // #G Deduction guide
    custom_take_view(R&& base, std::ranges::range_difference_t<R>)
        ->custom_take_view<std::ranges::views::all_t<R>>;

    namespace details {

        template<std::integral T>  // #A Only integrals
        struct custom_take_range_adaptor_closure {
            T count_;  // #B Store the count
            constexpr custom_take_range_adaptor_closure(T count)
                : count_{ count }
            {}

            // #C Allow it to be called with a range
            template<std::ranges::viewable_range R>
            constexpr auto operator()(R&& r) const
            {
                return custom_take_view(std::forward<R>(r), count_);
            }
        };

        struct custom_take_range_adaptor {
            template<typename... Args>
            constexpr auto operator()(Args&&... args)
            {
                if constexpr (sizeof...(Args) == 1) {
                    return custom_take_range_adaptor_closure{ args... };
                }
                else {
                    return custom_take_view{ std::forward<Args>(args)... };
                }
            }
        };


        template<std::ranges::viewable_range R, typename T>
        constexpr auto
            operator|(R&& r,
                const custom_take_range_adaptor_closure<T>& a)
        {
            return a(std::forward<R>(r));
        }
    }
}

namespace views {
    inline Cpp20RangesViewImplementationExample_01::details::custom_take_range_adaptor custom_take;
}

void ranges_04_examples()
{
    using namespace Cpp20RangesViewImplementationExample_01;
}

// ===========================================================================
// ===========================================================================

// namespace rg = std::ranges;

namespace Cpp20RangesViewImplementationExample_02
{
    // Original von Marius Bancila

    template<std::ranges::input_range R>
    requires std::ranges::view<R>
        class custom_take_view : public std::ranges::view_interface<custom_take_view<R>>
    {
    private:
        R                                         base_{};
        std::iter_difference_t<std::ranges::iterator_t<R>> count_{};
        std::ranges::iterator_t<R>                         iter_{ std::begin(base_) };
    public:
        custom_take_view() = default;

        constexpr custom_take_view(R base, std::iter_difference_t<std::ranges::iterator_t<R>> count)
            : base_(base)
            , count_(count)
            , iter_(std::begin(base_))
        {}

        constexpr R base() const&
        {
            return base_;
        }
        constexpr R base()&&
        {
            return std::move(base_);
        }

        constexpr auto begin() const
        {
            return iter_;
        }
        constexpr auto end() const
        {
            return std::next(iter_, count_);
        }

        constexpr auto size() const requires std::ranges::sized_range<const R>
        {
            const auto s = std::ranges::size(base_);
            const auto c = static_cast<decltype(s)>(count_);
            return s < c ? 0 : s - c;
        }
    };


    template<class R>
    custom_take_view(R&& base, std::iter_difference_t<std::ranges::iterator_t<R>>)
        ->custom_take_view<std::ranges::views::all_t<R>>;

    namespace details
    {
        struct custom_take_range_adaptor_closure
        {
            std::size_t count_;
            constexpr custom_take_range_adaptor_closure(std::size_t count) : count_(count)
            {}

            template <std::ranges::viewable_range R>
            constexpr auto operator()(R&& r) const
            {
                return custom_take_view(std::forward<R>(r), count_);
            }
        };

        struct custom_take_range_adaptor
        {
            template<std::ranges::viewable_range R>
            constexpr auto operator () (R&& r, std::iter_difference_t<std::ranges::iterator_t<R>> count)
            {
                return custom_take_view(std::forward<R>(r), count);
            }

            constexpr auto operator () (std::size_t count)
            {
                return custom_take_range_adaptor_closure(count);
            }
        };

        template <std::ranges::viewable_range R>
        constexpr auto operator | (R&& r, custom_take_range_adaptor_closure const& a)
        {
            return a(std::forward<R>(r));
        }
    }
}

namespace views
{
    Cpp20RangesViewImplementationExample_02::details::custom_take_range_adaptor custom_take2;
}

// ===========================================================================
// ===========================================================================

void ranges_ex_09_test_custom_view_01()
{
    using namespace Cpp20RangesViewImplementationExample_01;

    auto is_even = [](int const n) { return n % 2 == 0; };

    const std::vector<int> n{ 2, 3, 5, 6, 7, 8, 9 };

    auto v = n 
        | std::ranges::views::filter(is_even) 
        | views::custom_take(2);

    std::ranges::copy(v, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void ranges_ex_09_test_custom_view_02()
{
    using namespace Cpp20RangesViewImplementationExample_02;

    auto is_even = [](int const n) { return n % 2 == 0; };

    const std::vector<int> n{ 2, 3, 5, 6, 7, 8, 9 };

    auto v = n 
        | std::ranges::views::filter(is_even)
        | views::custom_take2(2);

    std::ranges::copy(v, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void ranges_ex_09_test_custom_view_03()
{
    using namespace Cpp20RangesViewImplementationExample_02;

    auto is_even = [](int const n) {return n % 2 == 0; };

    std::vector<int> n{ 1,1,2,3,5,8,13,21,34,55,89,144,233,377 };

    auto v = n 
        | std::ranges::views::filter(is_even) 
        | std::ranges::views::reverse 
        | views::custom_take(2);

    std::ranges::copy(v, std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

void ranges_04_view_implementation()
{
    ranges_ex_09_test_custom_view_01();
    ranges_ex_09_test_custom_view_02();
    ranges_ex_09_test_custom_view_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
