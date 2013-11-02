#!/opt/local/bin/perl


# remove duplicate bin files from the current directory
# this really should remove up to the first that changes
# then keep until they become the same. (preferring consecutive files)
#
# instead, i'll just delete all but the first one i encounter


#opendir ".", $dirname or die "Couldn't open dir: $!";
#my @files = readdir $dh;
#closedir $dh;

use File::Compare;

$|=1;

my @files = glob( "./*bin" );

$start = scalar @files;

foreach $file (@files)
{
	@otherfiles = glob( "./*bin" );
	foreach $otherfile( @otherfiles )
	{
		next if( $otherfile eq $file ); # don't check ourselves

		if( compare( $file, $otherfile ) == 0 ) {
			# they are equal.  Remove the second
			unlink( $otherfile );
			print ".";
		}
	}
}

# get the final count
@files = glob( "./*bin" );
$end = scalar @files;

printf "Scanned %d files, %d removed\n", $start, $start - $end ;
