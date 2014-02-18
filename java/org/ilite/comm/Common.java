package org.ilite.comm;

import java.nio.charset.Charset;

public class Common {
    /**
     * The charset encoding we use in C++
     */
    private static final String CHARSET_USED = "US-ASCII";

    /**
     * String header for binary chunks
     */
    private static final String STR_BIN_HEADER = "BIN\0";

    /**
     * Text header for binary chunks
     */
    private static final String STR_TXT_HEADER = "TXT\0";

    /**
     * Charset constant that uses CHARSET_USED. We needed to call forName()
     * enough that having a final instance for this was warranted.
     */
    public static final Charset TEXT_CHARSET = Charset.forName(CHARSET_USED);

    /**
     * STR_BIN_HEADER in bytes
     */
    public static final byte[] BIN_HEADER = STR_BIN_HEADER
            .getBytes(TEXT_CHARSET);

    /**
     * STR_TXT_HEADER in bytes
     */
    public static final byte[] TXT_HEADER = STR_TXT_HEADER
            .getBytes(TEXT_CHARSET);

    /**
     * Tells if reqMatch is what "with" starts with, starting at "offset" and
     * having length "length"
     * 
     * @param reqMatch
     *            - what needs to be contained entirely in with
     * @param with
     *            - array to check if it starts with "reqMatch"
     * @param offset
     *            - offset to use in "with"
     * @param length
     *            - length of "with"
     * @return true if "with" starts with reqMatch at the given offset, false
     *         otherwise
     */
    public static boolean matches(byte[] reqMatch, byte[] with, int offset,
            int length) {
        if (reqMatch == null || with == null) {
            return reqMatch == with;
        }

        if (with.length < offset + length) {
            throw new IllegalArgumentException(
                    "offset + length can't be > length of array");
        }

        if (reqMatch.length > length) {
            return false;
        }

        for (int i = 0; i < reqMatch.length; ++i) {
            if (reqMatch[i] != with[i + offset]) {
                return false;
            }
        }

        return true;
    }

    /**
     * {@see Common.matches(byte[], byte[], int int)}
     * 
     * @param reqMatch
     * @param with
     * @return
     */
    public static boolean matches(byte[] reqMatch, byte[] with) {
        if (reqMatch == null || with == null) {
            return reqMatch == with;
        }

        return Common.matches(reqMatch, with, 0, with.length);
    }
}
