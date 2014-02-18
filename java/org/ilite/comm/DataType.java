package org.ilite.comm;

import java.util.HashMap;

public enum DataType {
    Byte(0x00, "byte"),
    Boolean(0x01, "bool"),
    Char(0x02, "char"),
    ChunkObject(0x03, "chunk"),
    Double(0x04, "double"),
    Integer(0x05, "int"),
    String(0x06, "str"),
    Unreadable(0xFE, "UNREADABLE");

    private final byte m_value;
    private final String m_str;

    private static final HashMap<Integer, DataType> ilookupMap = new HashMap<Integer, DataType>();
    private static final HashMap<String, DataType> slookupMap = new HashMap<String, DataType>();

    static {
        final DataType[] dt = new DataType[] { Byte, Boolean, Char,
                ChunkObject, Double, Integer, String, Unreadable };

        for (final DataType d : dt) {
            ilookupMap.put(d.intValue(), d);
            slookupMap.put(d.strValue(), d);
        }
    }

    private DataType(int val, String s) {
        m_value = (byte) val;
        m_str = s;
    }

    /**
     * @return the value of a DataType as an int
     */
    public int intValue() {
        return m_value;
    }

    /**
     * @return the value of a DataType as a byte (this byte will be identical to
     *         the binary serialized representation of the type)
     */
    public byte byteValue() {
        return m_value;
    }

    /**
     * @return the value of a DataType as a String (this String will be
     *         identical to the Text serialized representation of the type)
     */
    public String strValue() {
        return m_str;
    }

    /**
     * Look up a DataType that has the given value.
     * 
     * @param val
     *            - value to look up
     * @return null if no type found, otherwise the corresponding DataType
     */
    public static DataType lookup(int val) {
        return ilookupMap.get(val);
    }

    /**
     * Look up a DataType that has the given String value.
     * 
     * @param val
     *            - value to look up
     * @return null if no type found, otherwise the corresponding DataType
     */
    public static DataType lookup(String s) {
        return slookupMap.get(s);
    }
}
