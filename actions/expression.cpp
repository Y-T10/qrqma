#include "expression.h"
#include "../demangle.h"
#include "../overloaded.h"

namespace qrqma {
namespace actions {
namespace detail {

template<typename L, typename R, typename op>
static void infix_helper(Context &context, Context::Expression lhs, Context::Expression rhs, op&& o) {
    auto f = [rhs=std::move(rhs), lhs=std::move(lhs), o=std::forward<op>(o)]{ return o(lhs.eval<L>(), rhs.eval<R>()); };
    context.pushExpression(typeid(decltype(f())), std::move(f));
}


template<typename op>
static bool exact_infix_helper(Context& context, Context::Expression& lhs, Context::Expression& rhs, op&& operation) {
    if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Integer, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Integer, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Bool, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Bool, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else {
        return false;
    }
    return true;
}

template<typename op>
static bool numeric_infix_helper(Context& context, Context::Expression& lhs, Context::Expression& rhs, op&& operation) {
    if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Integer, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::Integer, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Integer, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Float, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::Float, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Float, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Bool, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::Bool, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Bool, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else {
        return false;
    }
    return true;
}

template<typename op>
static bool complete_infix_helper(Context& context, Context::Expression& lhs, Context::Expression& rhs, op&& operation) {
    if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Integer, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::Integer, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Integer, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::String)) {
        infix_helper<types::Integer, types::String>(context, std::move(lhs), std::move(rhs), operation);

    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Float, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::Float, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Float, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Float) and rhs.type() == typeid(types::String)) {
        infix_helper<types::Float, types::String>(context, std::move(lhs), std::move(rhs), operation);

    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::Bool, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::Bool, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::Bool, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::Bool) and rhs.type() == typeid(types::String)) {
        infix_helper<types::Bool, types::String>(context, std::move(lhs), std::move(rhs), operation);

    } else if (lhs.type() == typeid(types::String) and rhs.type() == typeid(types::Integer)) {
        infix_helper<types::String, types::Integer>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::String) and rhs.type() == typeid(types::Float)) {
        infix_helper<types::String, types::Float>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::String) and rhs.type() == typeid(types::Bool)) {
        infix_helper<types::String, types::Bool>(context, std::move(lhs), std::move(rhs), operation);
    } else if (lhs.type() == typeid(types::String) and rhs.type() == typeid(types::String)) {
        infix_helper<types::String, types::String>(context, std::move(lhs), std::move(rhs), operation);
    } else {
        return false;
    }
    return true;
}

template<typename op>
static bool infix_helper_w_str(Context& context, Context::Expression& lhs, Context::Expression& rhs, op&& operation) {
    if (numeric_infix_helper(context, lhs, rhs, std::forward<op>(operation))) {
        return true;
    } else if (lhs.type() == typeid(types::String) and rhs.type() == typeid(types::String)) {
        infix_helper<types::String, types::String>(context, std::move(lhs), std::move(rhs), operation);
    } else {
        return false;
    }
    return true;
}

}

void action<grammar::ops::unary_minus>::apply(Context &context) {
    auto e = context.popExpression();
    if (e.type() == typeid(types::Integer)) {
        context.pushExpression(e.type(), [e_=std::move(e)]{
            return -e_.eval<types::Integer>();
        });
    } else if (e.type() == typeid(types::Float)) {
        context.pushExpression(e.type(), [e_=std::move(e)]{
            return -e_.eval<types::Float>();
        });
    } else if (e.type() == typeid(types::Bool)) {
        context.pushExpression(e.type(), [e_=std::move(e)]{
            return -e_.eval<types::Bool>();
        });
    } else {
        throw std::runtime_error("cannot apply unary minus to type: " + internal::demangle(e.type()));
    }
}

void action<grammar::ops::unary_not>::apply(Context &context) {
    auto e = context.popExpression();
    auto c = context.convert(e.type(), typeid(types::Bool));
    auto f = [c=std::move(c), e=std::move(e)]{ 
        return !std::any_cast<types::Bool>(c(e.eval_f()));
    };
    context.pushExpression(typeid(decltype(f())), std::move(f));
}

void action<grammar::ops::star>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l*r; };
    if (not detail::numeric_infix_helper(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator* to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::fslash>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l/r; };
    if (not detail::numeric_infix_helper(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator/ to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::percent>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l*r; };
    if (rhs.type() == typeid(types::Integer) and rhs.type() == typeid(types::Integer)) {
        detail::infix_helper<types::Integer, types::Integer>(context, std::move(lhs), std::move(rhs), op);
    } else {
        throw std::runtime_error("cannot apply operator% to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::plus>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) {
        return l+r;
    };
    if (detail::numeric_infix_helper(context, lhs, rhs, op)) {
        return;
    } else if (lhs.type() == typeid(types::String) and rhs.type() == typeid(types::String)) {
        detail::infix_helper<types::String, types::String>(context, std::move(lhs), std::move(rhs), op);
    } else {
        throw std::runtime_error("cannot apply operator+ to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::minus>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l-r; };
    if (not detail::numeric_infix_helper(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator+ to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::cmp_lt>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l<r; };
    if (not detail::infix_helper_w_str(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator< to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::cmp_leq>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l<=r; };
    if (not detail::infix_helper_w_str(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator<= to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::cmp_gt>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l>r; };
    if (not detail::infix_helper_w_str(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator> to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::cmp_geq>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l>=r; };
    if (not detail::infix_helper_w_str(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator>= to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::cmp_eq>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l==r; };
    if (not detail::infix_helper_w_str(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator== to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::cmp_neq>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = [](auto const& l, auto const& r) { return l==r; };
    if (not detail::infix_helper_w_str(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator!= to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::op_and>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = detail::overloaded{
        [](auto const& l, auto const& r) { return static_cast<bool>(l) && static_cast<bool>(r); },
        [](std::string const& l, std::string const& r) { return not l.empty() && r.empty(); },
        [](auto const& l, std::string const& r) { return static_cast<bool>(l) && r.empty(); },
        [](std::string const& l, auto const& r) { return not l.empty() && static_cast<bool>(r); },
    };
    if (not detail::complete_infix_helper(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator&& to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

void action<grammar::ops::op_or>::apply(Context &context) {
    auto rhs = context.popExpression();
    auto lhs = context.popExpression();
    auto op = detail::overloaded{
        [](auto const& l, auto const& r) { return static_cast<bool>(l) || static_cast<bool>(r); },
        [](std::string const& l, std::string const& r) { return not l.empty() || r.empty(); },
        [](auto const& l, std::string const& r) { return static_cast<bool>(l) || r.empty(); },
        [](std::string const& l, auto const& r) { return not l.empty() || static_cast<bool>(r); },
    };
    if (not detail::complete_infix_helper(context, lhs, rhs, op)) {
        throw std::runtime_error("cannot apply operator|| to " + internal::demangle(lhs.type()) + " and " + internal::demangle(rhs.type()));
    }
}

} // namespace actions
} // namespace qrqma