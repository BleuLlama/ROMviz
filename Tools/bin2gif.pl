#!/usr/bin/perl

$|=1;


sub doStuff
{
	$fn = shift;

	if( $fn eq "" ){
		print "Specify a filename.\n";
		exit( -1 );
	}

	printf "BIN: %s\n", $fn;
	$outfn = lc ($fn);
	$outfn =~ s/bin$/gif/g;
	printf "IMG: %s\n", $outfn;

	$sz = -s $fn;
	$w = 32;
	$h = 32;
	if( $sz == 32768 )
	{
		$w = 256;
		$h = 128;
	}

	if( $sz == 16384 )
	{
		$w = 128;
		$h = 128
	}
	#$sz = sqrt( -s $fn );

	$w = $w * 8;
	$h = $h * 8;

	printf "     %dx%d (%d)\n", $w, $h, $sz;

	open OF, ">TEMPFILE" or die "Couldn't write file.";
	printf OF "P1\n";
	printf OF "#from source file %s\n", $fn;
	printf OF "%d %d\n", $w, $h-1;

	open IF, "$fn" or die "Couldn't open source file.";

	$online = 0;
	for( $i=0 ; $i < (-s $fn ) ; $i ++ )
	{
		if( $online == 20 ) {
			$online = 0;
			printf OF "\n";
		} else {
			$online++;
		}
		read( IF, $v, 1 );
		$v = ord $v;
		if( $v & 0x80 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x40 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x20 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x10 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x08 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x04 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x02 ) { print OF "1 "; } else { print OF "0 "; }
		if( $v & 0x01 ) { print OF "1 "; } else { print OF "0 "; }
	}

	printf "Processed %d pixels\n", (-s $fn )*8;

	printf OF "\n";

	close IF;
	close OF;

	# cheat a bit
	`convert TEMPFILE $outfn`;
	#`rm TEMPFILE`;
}

$ac = 0;
while ( $ARGV[$ac] ne "" )
{
	doStuff( $ARGV[$ac] );
	$ac++;
}

