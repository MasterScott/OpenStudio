require 'date'

header = []
body = []
is_header = true
File.open('USA_CO_Golden-NREL.724666_TMY3.epw', 'r') do |epy|
  while line = epy.gets
    if is_header
      header << line
    else
      body << line
    end
    if /^DATA PERIODS/.match(line)
      is_header = false
    end
  end
end  

File.open('USA_CO_Golden-NREL.amy', 'w') do |amy|
  header.each do |line|
    if /DATA PERIODS/.match(line)
      amy.puts "DATA PERIODS,1,1,Data,Friday, 1/ 1,12/31"
    else
      amy.puts line
    end
  end
  
  d = DateTime.new(1999,1,1,1,0)
  ld = d
  body.each do |line|
    ld = d
    d += 1.0/24.0
    
    parts = line.split(',')
    if d.hour == 0
      parts[0] = ld.year
      parts[1] = ld.mon
      parts[2] = ld.mday
      parts[3] = 24
    else
      parts[0] = d.year
      parts[1] = d.mon
      parts[2] = d.mday
      parts[3] = d.hour
    end
    amy.puts parts.join(',')
  end 
end


File.open('USA_CO_Golden-NREL.wrap.amy', 'w') do |amy|
  header.each do |line|
    if /DATA PERIODS/.match(line)
      amy.puts "DATA PERIODS,1,1,Data,Saturday, 4/10, 4/ 9"
    else
      amy.puts line
    end
  end
  
  d = DateTime.new(1999,4,10,1,0)
  ld = d
  body.each do |line|
    ld = d
    d += 1.0/24.0
    
    parts = line.split(',')
    if d.hour == 0
      parts[0] = ld.year
      parts[1] = ld.mon
      parts[2] = ld.mday
      parts[3] = 24
    else
      parts[0] = d.year
      parts[1] = d.mon
      parts[2] = d.mday
      parts[3] = d.hour
    end
    amy.puts parts.join(',')
  end 
end