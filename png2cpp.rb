require 'chunky_png'
require 'rake/ext/string'

image = ChunkyPNG::Image.from_file ARGV.first

File.open(ARGV.last, 'w') do |file|
  file.puts '#include "image.h"'
  file.puts "extern const Image #{ARGV.last.pathmap('%{.png,}n')} {"
  file.puts "\t#{image.width}, #{image.height}, {\n"
  image.pixels.map {|p| ChunkyPNG::Color.to_hsl(p).last > 0.5 ? 1 : 0 }.each_slice(8).map do |bits|
    "0x#{(bits[7] << 7 | bits[6] << 6 | bits[5] << 5 | bits[4] << 4 | bits[3] << 3 | bits[2] << 2 | bits[1] << 1 | bits[0]).to_s(16)}"
  end.each_slice(128).map do |chars|
    file.write "\t"
    file.write chars.join(', ')
    file.write ",\n"
  end
  file.write "}};"
end
