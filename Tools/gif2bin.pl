#!/opt/local/bin/perl

sub processOne
{
	$sfn = shift;
	$outfn = lc $sfn;
	$outfn =~ s/gif$/bin/g;
	$tfn = "temp.pbm";

	printf "%s: Convert to %s\n", $sfn, $outfn;
	$cmd = sprintf "convert %s %s", $sfn, $tfn;
	`$cmd`;

	printf "%s: Stripping metadata\n", $outfn;
	open IF, "<:raw", "$tfn";
	binmode IF;
	open OF, ">$outfn";
	binmode OF;

	$junk = <IF>; #P5
#	$junk = <IF>; ## from whatever
#	$junk = <IF>; #
	$res = <IF>; #128 256

	($w, $h) = split / /,$res;
	printf "%s: image is %d x %d (%d px)\n", $sfn, $w, $h, ($w * $h);

	printf "Bytes read: %d\n", read IF, $content, ($w * $h );
	print OF $content;

	close OF;
	close IF;
}

$i = 0;
while( $ARGV[$i] ne "" )
{
	printf "\n";
	processOne( $ARGV[$i] );
	$junk = shift @ARGV;
}
printf "Done.\n";
