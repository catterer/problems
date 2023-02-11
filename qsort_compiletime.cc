#include <iostream>

// template<typename T>
// class Writer;

// template<>
// class Writer<int> {
// public:
//     static void write(int t) {
//         std::cout << "i" << t << "\n";
//     }
// };

// template<int i>
// class Writer<char[i]> {
// public:
//     static void write(const char* c) {
//         std::cout << c << "\n";
//     }
// };


// template<typename T, typename ...Args>
// void write(const T& t, Args&& ...args) {
//     Writer<T>::write(t);
//     if constexpr (sizeof...(args) > 0)
//         write(std::forward<Args>(args)...);
// }

template<typename T, T ...Elements>
struct List;

template<typename T, T Head, T ...Tail>
struct List<T, Head, Tail...> {
    using tail = List<T, Tail...>;
};

template<typename L>
struct Swap;

template<typename T, T el1, T el2, T ...Tail>
struct Swap<List<T, el1, el2, Tail...>> {
    using type = List<T, el2, el1, Tail...>;
};

template<typename La, typename Lb>
struct Concat;

template<typename T, T... TailA, T ...TailB>
struct Concat<List<T, TailA...>, List<T, TailB...>> {
    using type = List<T, TailA..., TailB...>;
};



template<bool B, typename T, typename F>
struct Cond {
    using type = T;
};

template<typename T, typename F>
struct Cond<false,T,F> {
    using type = F;
};

template<typename L, auto val,
     template<auto a, auto b> typename Pred>
struct Filter;

template<typename T, T Val, template<T a, T b> typename Pred>
struct Filter<List<T>, Val, Pred> {
    using type = List<T>;
};

template<typename T, T Head, T ...Tail, T Val, template<T a, T b> typename Pred>
struct Filter<List<T, Head, Tail...>, Val, Pred>{
    using filtered_tail = typename Filter<List<T, Tail...>, Val, Pred>::type;
    using type = typename Cond<
        Pred<Head, Val>::value,
        typename Concat<List<T, Head>, filtered_tail>::type,
        filtered_tail
    >::type;
};

template<int a, int b>
struct IntLess {
    static constexpr bool value = a < b;
};

template<int a, int b>
struct IntGE {
    static constexpr bool value = a >= b;
};




template<typename L>
struct QuickSelect;

template<typename T>
struct QuickSelect<List<T>> {
    using lesser = List<T>;
    using greater = List<T>;
};

template<typename T, T V, T ...Tail>
struct QuickSelect<List<T, V, Tail...>> {
    static constexpr T base = V;
    using lesser = typename Filter<List<T, Tail...>, V, IntLess>::type;
    using greater = typename Filter<List<T, Tail...>, V, IntGE>::type;
};

template<typename L>
struct Qsort;

template<typename T>
struct Qsort<List<T>> {
    using type = List<T>;
};

template<typename T, T V>
struct Qsort<List<T, V>> {
    using type = List<T, V>;
};

template<typename T, T Va, T Vb, T ...Tail>
struct Qsort<List<T, Va, Vb, Tail...>> {
    using QS = QuickSelect<List<T, Va, Vb, Tail...>>;
    using left = typename Qsort<typename QS::lesser>::type;
    using right = typename Qsort<typename QS::greater>::type;
    using type = typename Concat<left,typename Concat<List<T, QS::base>, right>::type>::type;
};

int main() {
    List<int, 1,2,3> l;
    static_assert(std::is_same<List<int, 2,1>::tail, List<int, 1>>::value);
    (void)l;

    static_assert(std::is_same<Swap<List<int, 1, 2, 3>>::type, List<int, 2, 1, 3>>::value);
    static_assert(std::is_same<Concat<List<int, 1, 2>, List<int, 3, 4>>::type, List<int, 1,2,3,4>>::value);
    static_assert(std::is_same<Concat<List<int>, List<int, 3, 4>>::type, List<int,3,4>>::value);
    static_assert(std::is_same<Concat<List<int>, List<int>>::type, List<int>>::value);
    static_assert(std::is_same<Filter<List<int, 1, 2, 3, 2>, 3, IntLess>::type, List<int, 1,2,2>>::value);
    static_assert(std::is_same<QuickSelect<List<int, 5, 1, 6, 2, 7>>::lesser, List<int, 1, 2>>::value);
    static_assert(std::is_same<QuickSelect<List<int, 5, 1, 6, 2, 7>>::greater, List<int, 6, 7>>::value);
    static_assert(std::is_same<Qsort<List<int, 5, 1, 6, 2, 7>>::type, List<int, 1, 2, 5, 6, 7>>::value);
    return 0;
}
