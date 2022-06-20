#pragma once

#define TRY_NULLABLE(nullable_expr) ({                                         \
    auto _nullable_val = nullable_expr;                                        \
    if (_nullable_val == nullptr) {                                            \
        nullptr;                                                               \
    }                                                                          \
    _nullable_val;                                                             \
})

#define TRY_NULLABLE_OR(nullable_expr, stmt) ({                                \
    auto _nullable_val = nullable_expr;                                        \
    if (_nullable_val == nullptr) {                                            \
        stmt;                                                                  \
    }                                                                          \
    _nullable_val;                                                             \
})

#define TRY_OPTION(option_expr) ({                                             \
    auto _option_val = option_expr;                                            \
    if (not _option_val.has_value()) {                                         \
        return {};                                                             \
    }                                                                          \
    *_option_val;                                                              \
})

#define TRY_OPTION_OR(option_expr, stmt) ({                                    \
    auto _option_val = option_expr;                                            \
    if (not _option_val.has_value()) {                                         \
        stmt;                                                                  \
    }                                                                          \
    *_option_val;                                                              \
})

#define TRY_RESULT(result_expr) ({                                             \
    auto _result_val = result_expr;                                            \
    if (_result_val.has_error()) {                                             \
        return cpp::failure{_result_val.error()};                              \
    }                                                                          \
    *_result_val;                                                              \
})

#define TRY_RESULT_OR(result_expr, stmt) ({                                    \
    auto _result_val = result_expr;                                            \
    if (_result_val.has_error()) {                                             \
        stmt;                                                                  \
    }                                                                          \
    *_result_val;                                                              \
})
