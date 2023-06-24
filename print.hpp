//
// Created by INS on 2023/6/23.
//

#ifndef PRINT_PRINT_HPP
#define PRINT_PRINT_HPP

#include <string>
#include <list>
#include <cstring>

namespace unip {
    namespace details {
        std::string to_hex(long long num) {
            char f[18]{0};
            sprintf(f, "0x%llx", num);
            return {f};
        }

        template<class T>
        struct print_wrapper {
            std::string operator()(const T &n) {
                return std::to_string(n);
            }
        };

        template<class T>
        struct print_wrapper<T *> {
            std::string operator()(const T *p) {
                return to_hex(reinterpret_cast<long long>(p));
            }
        };

        template<>
        struct print_wrapper<char *> {
            std::string operator()(const char *cs) {
                std::string s("\"");
                for (int i = 0; cs[i] != '\0'; ++i) {
                    s.push_back(cs[i]);
                }
                s.push_back('"');
                return s;
            }
        };

        template<>
        struct print_wrapper<bool> {
            std::string operator()(bool n) {
                return n ? "true" : "false";
            }
        };

        template<>
        struct print_wrapper<nullptr_t> {
            std::string operator()(nullptr_t) {
                return "nullptr";
            }
        };

        template<class R, class ...Arg>
        struct print_wrapper<R(*)(Arg...)> {
            std::string operator()(R(*p)(Arg...)) {
                return to_hex(reinterpret_cast<long long>(p));
            }
        };

        template<>
        struct print_wrapper<char> {
            std::string operator()(char c) {
                std::string s("'");
                s.push_back(c);
                s.push_back('\'');
                return s;
            }
        };

        template<>
        struct print_wrapper<std::string> {
            std::string operator()(const std::string &s) {
                return print_wrapper<char *>()(s.c_str());
            }
        };

        template<class T, class = void>
        struct is_support_iterator : std::false_type {
        };

        template<class T>
        struct is_support_iterator<T, std::void_t<std::decay_t<decltype(std::begin(std::declval<T>()))>,
                std::decay_t<decltype(std::end(std::declval<T>()))>>> : std::true_type {
        };

        template<class T>
        constexpr bool is_support_iterator_v = is_support_iterator<T>::value;

        template<typename C, typename = void>
        struct is_associative_container : std::false_type {
        };

        template<typename C>
        struct is_associative_container<C, std::void_t<typename C::key_type>> : std::true_type {
        };

        template<typename C>
        constexpr bool is_associative_container_v = is_associative_container<C>::value;


        template<class T, class = void>
        struct print_iterator {
            std::string operator()(const T &) {
                return "";
            }
        };

        template<class T>
        struct print_iterator<T, std::enable_if_t<is_support_iterator_v<T> && is_associative_container_v<T>>> {
            std::string operator()(const T &arr) {
                std::string s("{");
                for (const auto &[k, v]: arr) {
                    s += print_wrapper<std::decay_t<decltype(k)>>()(k) + ": ";
                    s += print_wrapper<std::decay_t<decltype(v)>>()(v);
                    s += ", ";
                }
                s.pop_back();
                s.pop_back();
                s.push_back('}');
                return s;
            }
        };

        template<class T>
        struct print_iterator<T, std::enable_if_t<is_support_iterator_v<T> && !is_associative_container_v<T>>> {
            std::string operator()(const T &arr) {
                std::string s("[");
                for (const auto &v: arr) {
                    s += print_wrapper<std::decay_t<decltype(v)>>()(v);
                    s += ", ";
                }
                s.pop_back();
                s.pop_back();
                s.push_back(']');
                return s;
            }
        };
    }

    template<class T>
    void print(const T &val) {
        using dt = std::decay_t<T>;
        if constexpr (std::is_fundamental_v<dt> || std::is_pointer_v<dt>) {
            if constexpr (std::is_pointer_v<dt>) {
                using noconstp = std::add_pointer_t<std::remove_cv_t<std::remove_pointer_t<dt>>>;
                std::cout << details::print_wrapper<noconstp>()(val) << std::endl;
            } else {
                std::cout << details::print_wrapper<dt>()(val) << std::endl;
            }
        } else if (std::is_enum_v<dt>) {
            std::cout << "enum";
        } else {
            if constexpr (std::is_same_v<dt, std::string> || std::is_same_v<dt, std::wstring>) {
                print(val.c_str());
            } else {
                std::cout << details::print_iterator<dt>()(val) << std::endl;
            }
        }
    }

    template<class T, size_t N, class = std::enable_if_t<!std::is_same_v<T, char>>>
    void print(const T (&arr)[N]) {
        std::cout << details::print_iterator<const T (&)[N]>()(arr) << std::endl;
    }
}

#endif //PRINT_PRINT_HPP
