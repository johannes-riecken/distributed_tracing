# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.0
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

package distributed_tracing;
use base qw(Exporter);
use base qw(DynaLoader);
package distributed_tracingc;
bootstrap distributed_tracing;
package distributed_tracing;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package distributed_tracing;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package distributed_tracing;

*from_edges_str = *distributed_tracingc::from_edges_str;

############# Class : distributed_tracing::pair_hash ##############

package distributed_tracing::pair_hash;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( distributed_tracing );
%OWNER = ();
%ITERATORS = ();
sub new {
    my $pkg = shift;
    my $self = distributed_tracingc::new_pair_hash(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        distributed_tracingc::delete_pair_hash($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package distributed_tracing;

1;
