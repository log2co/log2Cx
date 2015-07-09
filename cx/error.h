#ifndef ERROR_H
#define ERROR_H

namespace cx {
	extern int error_count;
	extern bool error_arrow_flag;
	extern int error_arrow_offset;

	///  Abort codes for fatal translator errors.

	enum abort_code {
		ABORT_INVALID_COMMANDLINE_ARGS = -1,
		ABORT_SOURCE_FILE_OPEN_FAILED = -2,
		ABORT_IFORM_FILE_OPEN_FAILED = -3,
		ABORT_ASSEMBLY_FILE_OPEN_FAILED = -4,
		ABORT_TOO_MANY_SYNTAX_ERRORS = -5,
		ABORT_STACK_OVERFLOW = -6,
		ABORT_CODE_SEGMENT_OVERFLOW = -7,
		ABORT_NESTING_TOO_DEEP = -8,
		ABORT_RUNTIME_ERROR = -9,
		ABORT_UNIMPLEMENTED_FEATURE = -10,
	};

	void abort_translation(abort_code ac);

	///  cx_error codes for syntax errors.

	enum error_code {
		ERR_NONE,
		ERR_UNRECOGNIZABLE,
		ERR_TOO_MANY,
		ERR_UNEXPECTED_EOF,
		ERR_INVALID_NUMBER,
		ERR_INVALID_FRACTION,
		ERR_INVALID_EXPONENT,
		ERR_TOO_MANY_DIGITS,
		ERR_REAL_OUT_OF_RANGE,
		ERR_INTEGER_OUT_OF_RANGE,
		ERR_MISSING_RIGHT_PAREN,
		ERR_INVALID_EXPRESSION,
		ERR_INVALID_ASSIGNMENT,
		ERR_MISSING_IDENTIFIER,
		ERR_MISSING_EQUAL,
		ERR_UNDEFINED_IDENTIFIER,
		ERR_STACK_OVERFLOW,
		ERR_INVALID_STATEMENT,
		ERR_UNEXPECTED_TOKEN,
		ERR_MISSING_SEMICOLON,
		ERR_MISSING_COMMA,
		ERR_MISSING_DO,
		ERR_MISSING_WHILE,
		ERRMISSINGTHEN, // DEPRECATED
		ERRINVALIDFORCONTROL, // DEPRECATED
		ERRMISSINGOF, // DEPRECATED
		ERR_INVALID_CONSTANT,
		ERR_MISSING_CONSTANT,
		ERR_MISSING_COLON,
		ERR_MISSING_LEFT_SUBSCRIPT,
		ERR_MISSING_RIGHT_SUBSCRIPT,
		ERR_REDEFINED_IDENTIFIER,
		ERR_MISSING_EQUAL_EQUAL,
		ERR_INVALID_TYPE,
		ERR_NOT_A_TYPE_IDENTIFIER,
		ERR_INVALID_SUBRANGE_TYPE, // DEPRECATED
		ERR_NOT_A_CONSTANT_IDENTIFIER,
		ERRMISSINGDOTDOT, // DEPRECATED
		ERR_INCOMPATIBLE_TYPES,
		ERR_INVALID_TARGET,
		ERR_INVALID_IDENTIFIER_USAGE,
		ERR_INCOMPATIBLE_ASSIGNMENT,
		ERRMINGTMAX, // DEPRECATED
		ERR_MISSING_LEFT_BRACKET,
		ERR_MISSING_RIGHT_BRACKET,
		ERR_INVALID_INDEX_TYPE,
		ERRMISSINGBEGIN, // DEPRECATED
		ERR_MISSING_RETURN,
		ERR_TOO_MANY_SUBSCRIPTS,
		ERR_INVALID_FIELD,
		ERR_NESTING_TOO_DEEP,
		ERRMISSINGPROGRAM, // DEPRECATED
		ERR_ALREADY_FORWARDED,
		ERR_WRONG_NUMBER_OF_PARMS,
		ERR_INVALID_REFERENCE,
		ERR_NOT_A_RECORD_VARIABLE,
		ERR_MISSING_VARIABLE,
		ERR_CODE_SEGMENT_OVERFLOW,
		ERR_UNIMPLEMENTED_FEATURE,
		ERR_MISSING_LEFT_PAREN,
		ERR_MISSING_SINGLE_QUOTE,
		ERR_INVALID_ESCAPE_CHAR,
		ERR_LOADING_LIBRARY,
		ERR_LIBRARY_NO_INIT
	};

	void cx_error(error_code ec);

	enum runtime_error_code {
		RTE_NONE,
		RTE_STACK_OVERFLOW,
		RTE_VALUE_OUT_OF_RANGE,
		RTE_INVALID_CASE_VALUE,
		RTE_DIVISION_BY_ZERO,
		RTE_INVALID_FUNCTION_ARGUMENT,
		RTE_INVALID_USER_INPUT,
		RTE_UNIMPLEMENTED_RUNTIME_FEATURE
	};

	void cx_runtime_error(runtime_error_code ec);
}
#endif
//endfig
