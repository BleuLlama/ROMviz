#!/usr/bin/perl

$|=1;

$fn = $ARGV[0];
if( $fn eq "" ){
	$fn = "roms.txt";
}

sub addStringToFile
{
	my $fn, $string;
	$fn = shift;
	$string = shift;

	open OF, ">>$fn";
	binmode OF;
	print OF $string;
	close OF;
}

sub extractData
{
	my $fn;
	$fn = shift;

	printf( "Scanning file: %s\n", $fn );
	open IF, "$fn";

	$filename = "unk";
	$nstarts = 0;
	$phase = 0;
	$size = 0;
	$writtenSize = 0;

	foreach $line (<IF>)
	{
		$line =~ s/[\s]$//g;

		if( $phase == 3 ) {
			# get data bytes
			($addr, $data) = split '\s', $line;
			printf( "." );

			$binData = $data;
			#$binData =~ s/(..)/chr(hex($1))/ge;
			$binData =~ s/([a-f0-9][a-f0-9])/chr(hex($1))/egi;

			# should probably replace this with write() but this works
			addStringToFile( $filename, $binData );
			$writtenSize += (length( $data )/2 );
			if( $writtenSize >= $size ) {
				$phase = 0;
			}
		}

		if( $phase == 2 ) {
			# get bytes line
			$size = hex $line ;
			printf( "	Size: %d (0x%s) bytes\n", $size, $line );
			$phase = 3;
			unlink( $filename ) 
		}

		if( $phase == 1 ) {
			# get time line
			printf( "	Time: %d ms\n", $line );
			$phase = 2;
			$filename = sprintf( "%s_%07d.bin", $fn, $line );
		}

		if( -1 ne index $line, ".oOo.oOo." ) {
			# found carrot
			$nstarts++;
			$phase = 1;
			printf( "\n\n" );
			$writtenSize = 0;
		}
	}

	printf( "\nSaved %d candidates.\n",$nstarts );

	close IF;
}

sub main
{
	extractData( $fn );
}

&main;
