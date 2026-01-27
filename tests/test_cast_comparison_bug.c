/*
 * Test case for cast comparison type safety bug
 * Issue from TODO: "fix invalid cast in comparison 'if (v == (int8_t)v)'"
 * 
 * This test demonstrates the problem where type casting in comparisons
 * can produce incorrect results due to improper handling of truncation.
 */

#include <unistd.h>

/* Write string to stdout */
static void write_str(const char *str) {
    int len = 0;
    const char *p = str;
    while (*p) {
        len++;
        p++;
    }
    if (len > 0)
        write(1, str, len);
}


/* Test Case 1: Signed char truncation */
int test_signed_char_cast_comparison(void)
{
    int value = 256;        /* 0x100 - will truncate to 0 when cast to signed char */
    
    write_str("Test 1: value = 256, casting to signed char (8-bit)\n");
    write_str("Expected: 256 != (char)256\n");
    
    /* BUG: This comparison should be FALSE */
    if (value == (char)value) {
        write_str("  FAIL: 256 == (char)256 evaluated to TRUE (WRONG)\n");
        return 1;  /* Bug exists */
    } else {
        write_str("  PASS: 256 == (char)256 correctly evaluated to FALSE\n");
        return 0;  /* Bug fixed */
    }
}

/* Test Case 2: Unsigned char truncation */
int test_unsigned_char_cast_comparison(void)
{
    int value = 300;        /* 0x12C - will truncate to 44 */
    
    write_str("Test 2: value = 300, casting to unsigned char\n");
    write_str("Expected: 300 != (unsigned char)300\n");
    
    /* BUG: This comparison should be FALSE */
    if (value == (unsigned char)value) {
        write_str("  FAIL: 300 == (unsigned char)300 evaluated to TRUE (WRONG)\n");
        return 1;  /* Bug exists */
    } else {
        write_str("  PASS: 300 == (unsigned char)300 correctly evaluated to FALSE\n");
        return 0;  /* Bug fixed */
    }
}

/* Test Case 3: Control case - value that fits */
int test_no_truncation(void)
{
    int value = 100;
    
    write_str("Test 3: value = 100 (fits in char)\n");
    write_str("Expected: 100 == (char)100\n");
    
    /* This SHOULD be TRUE */
    if (value == (char)value) {
        write_str("  PASS: 100 == (char)100 correctly evaluated to TRUE\n");
        return 0;  /* Correct */
    } else {
        write_str("  FAIL: 100 == (char)100 evaluated to FALSE (WRONG)\n");
        return 1;  /* Wrong */
    }
}

int main(void)
{
    int total_failures = 0;
    
    write_str("======================================================\n");
    write_str("TinyCC Cast Comparison Bug Test\n");
    write_str("Issue: Invalid cast in comparison from TODO\n");
    write_str("======================================================\n\n");
    
    total_failures += test_signed_char_cast_comparison();
    write_str("\n");
    
    total_failures += test_unsigned_char_cast_comparison();
    write_str("\n");
    
    total_failures += test_no_truncation();
    write_str("\n");
    
    write_str("======================================================\n");
    if (total_failures == 0) {
        write_str("RESULT: ALL TESTS PASSED - Bug is fixed!\n");
    } else {
        write_str("RESULT: Bug still exists!\n");
    }
    write_str("======================================================\n");
    
    return total_failures;
}
