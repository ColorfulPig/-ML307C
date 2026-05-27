#!/usr/bin/perl
#
#[20190422][xiaokeweng@asrmicro.com]
# COMM: NEW_LOADTABLE refactor for CRANE project, recorder the binary size (might RW compressed)
#       into loadtable for boot33 to detect the truly tail address of CP image inside nor-flash.
#       and then boot33 able to caculate the most effective nvm-initial-start address just append 
#       the CP.bin in the same "cp" partation.
#
#[20190423][xiaokeweng@asrmicro.com]
# COMM: update the GENERAL_INFO fillin , the GENERAL_INFO_STRING should be sync with /csw/BSP/src/loadtable.c
#       and the IMG_GENERAL_INFO_LENGTH = 64 also refer to loadtable.c ,

use Fcntl;

$DEBUG=1;
$LOADTABLE_HEADER_LENGTH=0x1200;
$BINARY_SIZE_STRING="BIN_SIZE";
$GENERAL_INFO_STRING="IMAGE_GENERAL_INFO";
$IMG_FILENAME = $ARGV[0];
$IMG_GENERAL_INFO = $ARGV[1];
$IMG_GENERAL_INFO_LENGTH=64;

my $binary_size= -s $IMG_FILENAME;
sysopen(INPUT_STREAM,"$IMG_FILENAME",O_BINARY|O_RDWR)
	or error_exit("Can't open [$IMG_FILENAME] for reading; $!");

#print sprintf ("binary_size=[0x%.08x]\n", $binary_size);
sysread(INPUT_STREAM,$loadtable_header,$LOADTABLE_HEADER_LENGTH);
$index=index($loadtable_header,$BINARY_SIZE_STRING);
if ($index == -1){
	error_exit("NO BINARY_SIZE_STRING [$BINARY_SIZE_STRING] detect, exit");
}

sysseek(INPUT_STREAM,$index,0);
sysread(INPUT_STREAM,$ITEM_NAME,12);
sysseek(INPUT_STREAM,$index+12,0);
sysread(INPUT_STREAM,$original_val,4);
#
#print sprintf ("original_val =[0x%.08x  ]\r\n",raw_to_hex($original_val));

# load the first {$LOADTABLE_HEADER_LENGTH} binary to parse
# check {$BINARY_SIZE_STRING} flag which will fill in the trully image size (might RW compressed)
#
#/csw/BSP/inc/loadTable.h
#   typedef struct{
#       char    name[12];
#       UINT32  value;
#   }armlink_symbol_item;
#
#
sysseek(INPUT_STREAM,$index+12,0);
$tmp_raw = hex_32_reorder($binary_size);
syswrite(INPUT_STREAM,pack("H*",sprintf("%08x",$tmp_raw)),4);

sysseek(INPUT_STREAM,$index+12,0);
sysread(INPUT_STREAM,$filled_in_val,4);
#print sprintf ("filledin_val =[0x%.08x  ]\r\n",raw_to_hex($filled_in_val));

#print sprintf ("[%12s]=[0x%.08x  ] -> [0x%.08x  ]\r\n",
#	$ITEM_NAME,raw_to_hex($original_val),raw_to_hex($filled_in_val));

$index=index($loadtable_header,$GENERAL_INFO_STRING);
if ($index == -1){
	error_exit("NO GENERAL_INFO_STRING [$GENERAL_INFO_STRING] detect, exit");
}

#sysseek(INPUT_STREAM,$index,0);
#print sprintf ("GENERAL_INFO index = [0x%.08x  ]\r\n",($index));
#sysread (INPUT_STREAM,$general_info_buf,$IMG_GENERAL_INFO_LENGTH);
#print sprintf ("GENERAL_INFO AREA DUMP 1 = [%s  ]\r\n",($general_info_buf));

my $count=0;
$tmp_raw = hex_32_reorder(0x00000000);
while($count < $IMG_GENERAL_INFO_LENGTH){
	sysseek(INPUT_STREAM,$index+$count,0);
	syswrite(INPUT_STREAM,pack("H*",sprintf("%08x",$tmp_raw)),4);
	$count += 4;
}

#sysseek(INPUT_STREAM,$index,0);
#sysread (INPUT_STREAM,$general_info_buf,$IMG_GENERAL_INFO_LENGTH);
#print sprintf ("GENERAL_INFO AREA DUMP 2 = [%s  ]\r\n",($general_info_buf));

#print sprintf ("IMG_GENERAL_INFO   = [%s  ]\r\n",($IMG_GENERAL_INFO));

sysseek(INPUT_STREAM,$index,0);
syswrite(INPUT_STREAM,$IMG_GENERAL_INFO,$IMG_GENERAL_INFO_LENGTH);

#sysseek(INPUT_STREAM,$index,0);
#sysread (INPUT_STREAM,$general_info_buf,$IMG_GENERAL_INFO_LENGTH);
#print sprintf ("GENERAL_INFO AREA DUMP 3 = [%s  ]\r\n",($general_info_buf));



close INPUT_STREAM;

# functions
#
sub hex_32_reorder{
   local $m_hex = $_[0];
   $m_hex = ((($m_hex&0x000000ff)<<24)|
		(($m_hex&0x0000ff00)<<8)|
		(($m_hex&0x00ff0000)>>8)|
		(($m_hex&0xff000000)>>24));
   return $m_hex;
}

sub raw_to_hex{
   local $m_hex = hex(unpack("H*",$_[0]));
   $m_hex = hex_32_reorder($m_hex);
   return $m_hex;
}

sub error_exit{
	local $ERROR_MSG="\nERROR:**";
	print "----------------------";
	print "$ERROR_MSG @_";
	print "\n";
	exit(1);
}
