#!/usr/bin/perl

#
#[20190307][xiaokeweng@asrmicro.com]
# COMM: Tnitial perl script ,do (RW+ZI) region compress. The header of
#       image which to fetch compress area is recordered from C code ,
#       may refer to /csw/bsp/src/loadTable.c for more detail.
# TODO: Armlink could do RW region compress during armlink
#       but our scatter file did include ZI data inside into the RW region.
#       Do further check to refactor the scatter file.
#
#[20190309][xiaokeweng@asrmicro.com]
# COMM: correct typo & fix issue which LZOP compressed info inside split_result[6]
#       did not count the LZOP header&tail part size inside. it only caculate for
#       compress ratio. Using [-s <file>] to get the true size of compressed file.
#
#[20190310][xiaokeweng@asrmicro.com]
# COMM: bad CPZ_LOAD_ADDR raw data, missing the initial zero address after hex_32_reorder
#       fix with format-output during syswrite.


#[20201017][xiaokeweng@asrmicro.com]
# COMM: implement file_merge func to avoid CAT/COPY switch in defferent host env.
#
use Fcntl;
use File::Basename;
use File::Spec::Functions;

$DEBUG=0;
$LOADTABLE_HEADER_LENGTH=1024;
$CPZ_MARK_STRING="RW_CPZ_";
$IMG_FILENAME = $ARGV[0];
$PART_1 = "part1";
$PART_2 = "part2";
$TMP = "tmp";
$TMP_LZOP = "tmp_lzop";
#$INIT_LOAD_ADDRESS=0x80200000;
$INIT_LOAD_MARK_STRING="CP_EXEC";
$PAD_LENGTH=4096;

if ($^O eq 'MSWin32' || $^O eq 'msys' || $^O eq 'cygwin') {
	$EXE_SUFFIX = ".exe";
} else {
	$EXE_SUFFIX = "_lnx";
}
$LZOP = catfile(dirname(__FILE__), "asr_lzop$EXE_SUFFIX");

sysopen(INPUT_STREAM,"$IMG_FILENAME",O_BINARY|O_RDWR)
	or error_exit("Can't open [$IMG_FILENAME] for reading; $!");
print sprintf ("|%-8s|%-74s\n","INPUT",$IMG_FILENAME);

# load the frist {$LOADTABLE_HEADER_LENGTH} binary to parse
# check {$INIT_LOAD_MARK_STRING} flag which appends the cp core execute address
#
#/csw/BSP/inc/loadTable.h
#   typedef struct{
#       char    name[12];
#       UINT32  value;
#   }armlink_symbol_item;
#
#
sysread(INPUT_STREAM,$loadtable_header,$LOADTABLE_HEADER_LENGTH);
$index=index($loadtable_header,$INIT_LOAD_MARK_STRING);
if ($index == -1){
	error_exit("NO compress string [$INIT_LOAD_MARK_STRING] detect, exit");
}else{
	sysseek(INPUT_STREAM,$index+12,0);
	sysread(INPUT_STREAM,$INIT_LOAD_ADDRESS,4);
	$INIT_LOAD_ADDRESS = raw_to_hex($INIT_LOAD_ADDRESS);
	#print sprintf ("0x%.08x\n",$INIT_LOAD_ADDRESS);
}

# check {$CPZ_MARK_STRING} flag which repsent the regiong compress info
#
#debug_msg("loadtable_header:[\n$loadtable_header\n]");
$index=index($loadtable_header,$CPZ_MARK_STRING);
debug_msg("index=$index");
if ($index == -1){
	error_exit("NO compress string [$CPZ_MARK_STRING] detect, exit");
}else{

	#open OUTBIN_P1,">$IMG_FILENAME\_$PART_1" or error_exit("can't open $IMG_FILENAME\_$PART_1 for output");
	sysopen(OUTBIN_P1,"$IMG_FILENAME\_$PART_1",O_BINARY|O_CREAT|O_RDWR);
	debug_msg("create the empty $IMG_FILENAME\_$PART_1");
	close OUTBIN_P1;

	#open OUTBIN_P2,">$IMG_FILENAME\_$PART_2" or error_exit("can't open $IMG_FILENAME\_$PART_2 for output");
	sysopen(OUTBIN_P2,"$IMG_FILENAME\_$PART_2",O_BINARY|O_CREAT|O_RDWR);
	debug_msg("create the empty $IMG_FILENAME\_$PART_2");
	close OUTBIN_P2;
};

# parse each item start with $CPZ_MARK_STRING,
# the format detail refer to csw/BSP/inc/loadTable.h
#
#typedef struct
#{
# char    RW_REGION_MARK[8];
# char    RW_REGION_NAME[8];
# UINT32  RW_REGION_EXEC_ADDR;
# UINT32  RW_REGION_LOAD_ADDR;
# UINT32  RW_REGION_LENGTH;
# UINT32  RW_REGION_COMPRESSED_ADDR;
#}rw_region_item;
#
print sprintf ("|%-8s|%-8s|%-8s.%-8s.%-8s.%-8s|%-30s|\n",
               "MARK","NAME","EXEADDR","LOADADDR","LENGTH","CPZLADDR","COMPRESS STASTIC");
print sprintf ("|%-8s|%-8s|%-8s.%-8s.%-8s.%-8s|%-30s|\n",
               "--------","--------","--------","--------","--------","--------","------------------------------");
sysseek(INPUT_STREAM,$index,0);
while(1){
	sysread(INPUT_STREAM,$mark,8);debug_msg("mark = $mark");
	unless($mark =~ /$CPZ_MARK_STRING/){last;}
	sysread(INPUT_STREAM,$name,8);
	sysread(INPUT_STREAM,$exec_addr,4);$exec_addr = raw_to_hex($exec_addr);
	sysread(INPUT_STREAM,$load_addr,4);$load_addr = raw_to_hex($load_addr);
	sysread(INPUT_STREAM,$length,4);$length = raw_to_hex($length);
	sysread(INPUT_STREAM,$cpz_load_addr,4);$cpz_load_addr = raw_to_hex($cpz_load_addr);

	# recorder the first compress region offset for PART_1
	#
	unless( "$mark" cmp "$CPZ_MARK_STRING".1){
		$p1_length=($load_addr-$INIT_LOAD_ADDRESS);
		$tmp_cpz_load_addr=($load_addr);
		debug_msg("p1_length=$p1_length");
		if($p1_length&($PAD_LENGTH-1)){
			$p1_pad_length = $PAD_LENGTH - $p1_length&($PAD_LENGTH-1);
			#print sprintf ("p1_pad_length %.8x\n",$p1_pad_length);
			$tmp_cpz_load_addr = $tmp_cpz_load_addr + $p1_pad_length;
			#print sprintf ("tmp_cpz_load_addr %.8x\n",$tmp_cpz_load_addr);
		}
		
	}

	#print sprintf ("[%8s][%8s][0x%.08x][0x%.08x][0x%.08x]\n",$mark,$name,$exec_addr,$load_addr,$length);

	# step1: cut uncompressed Region into OUTBIN_TMP file
	#
	open TMP_INPUT_STREAM, $IMG_FILENAME or error_exit("Can't open [$IMG_FILENAME] for reading; $!");
	sysopen(TMP_INPUT_STREAM,$IMG_FILENAME,O_BINARY|O_RDONLY);
	sysseek(TMP_INPUT_STREAM,$load_addr-$INIT_LOAD_ADDRESS,0);
	sysread(TMP_INPUT_STREAM,$buf,$length);
	sysopen(OUTBIN_TMP,"$mark\_$TMP",O_BINARY|O_CREAT|O_WRONLY);
	syswrite(OUTBIN_TMP,$buf,$length);
	close TMP_INPUT_STREAM;
	close OUTBIN_TMP;

	# step2: compressed the OUTBIN_TMP & get the lzop info
	#
	$lowercase_tmp_lzop_file=lc("$mark\_$TMP_LZOP"); # set to lowercase to avoid case-change under linux env , with WINE lzop.exe invoke
	qx("$LZOP" -9fv -o $lowercase_tmp_lzop_file $mark\_$TMP  2>&1 );
	
	$tmp_lzop_size=-s "$lowercase_tmp_lzop_file";
	sysopen(OUTBIN_TMP_LZOP,"$lowercase_tmp_lzop_file",O_BINARY|O_WRONLY|O_APPEND);
	if($tmp_lzop_size&($PAD_LENGTH-1)){
		$temp_pad_length = $PAD_LENGTH - $tmp_lzop_size&($PAD_LENGTH-1);
		#print sprintf ("temp_pad_length %.8x\n",$temp_pad_length);
		while($temp_pad_length--){
			#print OUTBIN_TMP_LZOP '0';
			syswrite(OUTBIN_TMP_LZOP,pack("H1","0"),1);
		}
		#syswrite(OUTBIN_TMP_LZOP,$bufpad,$temp_pad_length);
	}
	close OUTBIN_TMP_LZOP;
	
	$tmp_file_size=-s "$mark\_$TMP";
	$tmp_lzop_size=-s "$lowercase_tmp_lzop_file";

	print sprintf ("|%8s|%7s |%.08x.%.08x.%.08x.%.08x|%.08x->%.08x\n",
               $mark,substr($name,0,7),$exec_addr,$load_addr,$length,$tmp_cpz_load_addr,
               $tmp_file_size,$tmp_lzop_size);

	# step3: record the current load addr into image header
	#        if (==1st ) RW_REGION_COMPRESSED_ADDR = RW_REGION_LOAD_ADDR;
	#        if (>=2nd ) RW_REGION_COMPRESSED_ADDR = last(RW_REGION_COMPRESSED_ADDR)+last(COMPRESSED_LENGTH);
	#
	my $tell=sysseek(INPUT_STREAM,0,1);

	sysseek(INPUT_STREAM,$tell-8,0);
	debug_msg(sysseek(INPUT_STREAM,0,1));
	$tmp_size = hex_32_reorder($tmp_lzop_size);
	#print sprintf "debug - tmp_raw=0x%.08x\n",$tmp_raw;
	syswrite(INPUT_STREAM,pack("H*",sprintf("%08x",$tmp_size)),4);

	debug_msg(sysseek(INPUT_STREAM,0,1));
	$tmp_raw = hex_32_reorder($tmp_cpz_load_addr);
	#print sprintf "debug - tmp_raw=0x%.08x\n",$tmp_raw;
	syswrite(INPUT_STREAM,pack("H*",sprintf("%08x",$tmp_raw)),4);

	$tmp_cpz_load_addr = $tmp_cpz_load_addr+$tmp_lzop_size;
	#print sprintf ("tmp_cpz_load_addr=[%.08x][%d]\n",$tmp_cpz_load_addr,$tmp_cpz_load_addr);

	# step4: append compressed file into $IMG_FILENAME\_PART_2 one by one
	#
	sysopen(OUTBIN_TMP_LZOP,"$lowercase_tmp_lzop_file",O_BINARY|O_RDWR);
	sysopen(OUTBIN_P2,"$IMG_FILENAME\_$PART_2",O_BINARY|O_WRONLY|O_APPEND);
	while(<OUTBIN_TMP_LZOP>){
		print OUTBIN_P2 $_;
	}

	# step5: trash clean
	#
	unlink glob("$mark*");
	close OUTBIN_P2;
	close OUTBIN_TMP_LZOP;
}

#print sprintf ("[%-8s][%-8s][%-8s][%-8s][%-8s][%-8s][%-33s]\n",
#               "--------","--------","--------","--------","--------","--------","---------------------------------");
unlink glob("*$TMP_LZOP");

# step6: append compressed file into $IMG_FILENAME\_PART_2 one by one
#
sysseek(INPUT_STREAM,0,0);
sysopen(INPUT_STREAM,"$IMG_FILENAME",O_BINARY|O_RDONLY);
sysread(INPUT_STREAM,$buf,$p1_length);
sysopen(OUTBIN_P1,"$IMG_FILENAME\_$PART_1",O_BINARY|O_WRONLY|O_APPEND);
syswrite(OUTBIN_P1,$buf,$p1_length);
if($p1_length&($PAD_LENGTH-1)){
	$p1_pad_length = $PAD_LENGTH - $p1_length&($PAD_LENGTH-1);
	#print sprintf ("p1_pad_length %.8x\n",$p1_pad_length);
	while($p1_pad_length--){
		#print OUTBIN_P1 '0';
		syswrite(OUTBIN_P1,pack("H1","0"),1);
	}
	#syswrite(OUTBIN_P1,$bufpad,$p1_pad_length);
}


# step6: PART1 + PART2 > IMG_FILENAME & dump IMAGE size compress summery
#
#print sprintf ("[%-8s][%-8s][%-8s][%-8s][%-8s][%-8s][%-33s]\n",
#               "--------","--------","--------","--------","--------","--------","---------------------------------");
my $original_size= -s $IMG_FILENAME;
unlink("$IMG_FILENAME");
close INPUT_STREAM;

file_merge("$IMG_FILENAME\_$PART_1","$IMG_FILENAME\_$PART_2",$IMG_FILENAME);

my $compressed_size= -s $IMG_FILENAME;

print sprintf ("|%-8s|%-8s|%-8s.%-8s.%-8s.%-8s|%-30s|\n",
               "--------","--------","--------","--------","--------","--------","------------------------------");
print sprintf ("|%-53s|%6s0x%.08x -> 0x%.08x|\n","","",
      $original_size,$compressed_size);
print sprintf ("|%-53s|%6s%6.3f(MB) -> %6.3f(MB)|\n","","",
      $original_size/1024/1024,$compressed_size/1024/1024);
print sprintf ("|%s|\n","------------------------------------------------------------------------------------");

close OUTBIN_P1;

unlink("$IMG_FILENAME\_$PART_1");
unlink("$IMG_FILENAME\_$PART_2");
exit(0);

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

sub debug_msg{
	unless($DEBUG){return};
	local $DEBUG_MSG="DEBUG >>> :";
	print "$DEBUG_MSG @_";
	print "\n";
}
sub error_exit{
	local $ERROR_MSG="\nERROR:**";
	print "----------------------";
	print "$ERROR_MSG @_";
	print "\n";
	exit(1);
}

sub file_merge{
	open(RH,$_[0]) or die "$!";
	binmode RH;
	my @src_1=<RH>;
	close RH;

	open(RH,$_[1]) or die "$!";
	binmode RH;
	my @src_2=<RH>;
	close RH;

	#create&overwrite for input file arg0
	open(WH,">$_[2]") or die "$!";
	binmode WH;
	my $i;
	for($i = 0; $i < @src_1; $i++){
		print WH $src_1[$i];
	}
	close WH;

	#append for input file arg1
	open(WH,">>$_[2]") or die "$!";
	binmode WH;
	my $i;
	for($i = 0; $i < @src_2; $i++){
		print WH $src_2[$i];
	}
	close WH;

}
