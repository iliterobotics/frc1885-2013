#!/usr/bin/env ruby

def typemap
  { 0x00 => :byte,
    0x01 => :bool,
    0x02 => :char,
    0x03 => :chunk,
    0x04 => :double,
    0x05 => :integer,
    0x06 => :string }
end

def typesizes
  { :byte => 1,
    :bool => 1,
    :char => 1,
    :double => 8,
    :integer => 4 }
end

def str_to_bytes(str)
    # Split into lists of two chars each

  inter = str.chars.reduce([[]]) do |accum, chr|
    if accum[-1].size == 2 then
      accum << [chr]
    else
      accum[-1] << chr
    end
    accum
  end

  inter.flat_map { |n| n.reduce(:+).hex }
end

def next_field_type(bytez)
  return nil if bytez.nil? or bytez.empty?

  raise RuntimeError,"Incomplete type information" if bytez.size < 2

  sized = bytez[0] & 0x80
  type = bytez[1]

  raise RuntimeError,"Invalid field type." unless typemap.has_key? type

  typesym = typemap[type]

  if sized == 0
    raise RuntimeError,"Field lacks size" unless typesizes.has_key? typesym

    { :type => typemap[type], :size => typesizes[typesym], :consumed => 2 }
  else
    raise RuntimeError,"Too small to have size" if bytez.size < 6

    size = bytez[2,4].reduce { |a, c| (a << 8) | c }

    { :type => typesym, :consumed => 6, :size => size }
  end
end

def next_field_value(size, type, bytez)
  raise RuntimeError,"Not enough bytes remaining to satisfy size" unless bytez.size >= size

  resultb = bytez[0,size]

  case type
  when :integer
    resultb.reduce { |a, n| (a << 8) | n }
  when :double
    # TODO: I have no clue how to reassemble a binary float in Ruby.
    resultb.reduce("") { |a, n| a + n.to_s(16).rjust(2, '0') }
  when :bool
    resultb.reduce(:+) != 0
  when :string
    resultb.reduce("") { |a, n| a + n.chr }
  when :chunk
    resultb.reduce("") { |a, n| a + n.to_s(16).rjust(2, '0') }
  else
    raise RuntimeError,"Unexpected Type #{ type.to_s }"
  end
end

def is_bin_chunk?(bytez)
  return false if bytez.nil? or bytez.size < 4

  bin_str = "BIN\0".chars.map { |x| x.ord }

  bin_str == bytez[0, 4]
end

def handle_bstring(st, raw_prn = true, level = 0)
  bytez = str_to_bytes(st)
  chrs = 4

  return if bytez.nil?

  unless is_bin_chunk?(bytez)
    print "Not a binary chunk.\n" if raw_prn
    return
  end

  bytez = bytez[4, bytez.size]

  prefix = "\t" * level

  begin
    until bytez.empty?
      nft = next_field_type(bytez)
      chrs += nft[:consumed]

      break if nft.nil?

      # Remove number of consumed bytes
      bytez = bytez[nft[:consumed], bytez.size]

      res = next_field_value(nft[:size], nft[:type], bytez)
      chrs += nft[:size]

      if nft[:type] == :chunk and is_bin_chunk?(str_to_bytes(res))
        print "#{ prefix }Binary chunk:\n"
        handle_bstring(res, false, level + 1)
      else
        print "#{ prefix }#{ nft[:type] }: Value: #{ res } \n"
      end

      bytez = bytez[nft[:size], bytez.size]
    end
  rescue RuntimeError => err
    print "Error #{ err.to_s } around char #{ chrs } in #{ st } (#{ bytez.reduce("") { |a, n| a += n.to_s(16) } })\n"
  end
end

handle_bstring ARGV.shift unless ARGV[0].nil?
