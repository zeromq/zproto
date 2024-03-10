%global debug_package %{nil}
Name:           zproto
Version:        0.0.1
Release:        1
Summary:        Protocol framework for ZeroMQ
License:        MIT
URL:            https://github.com/zeromq/zproto
Source0:        %{name}-%{version}.tar.gz
Group:          System/Libraries
# Note: ghostscript is required by graphviz which is required by
#       asciidoc. On Fedora 24 the ghostscript dependencies cannot
#       be resolved automatically. Thus add working dependency here!
BuildRequires:  ghostscript
BuildRequires:  asciidoc
BuildRequires:  automake
BuildRequires:  autoconf
BuildRequires:  libtool
BuildRequires:  pkgconfig
BuildRequires:  xmlto
BuildRequires:  generator-scripting-language
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       generator-scripting-language

%description
 zproto is a set of code generators that can produce:
 fast and efficient binary codecs for ZeroMQ-based protocols.
 full-featured protocol servers based on high-level state machine models.
 full-featured protocol clients based on high-level state machine models.


%prep
%setup -q

%build
sh autogen.sh
%{configure}
make %{_smp_mflags}

%install
make install DESTDIR=%{buildroot} %{?_smp_mflags}

%files
%defattr(-,root,root)
%{_bindir}/*.gsl
%{_mandir}/man7/*
%doc README.md
%doc README.txt
%doc scripts/*

%package -n %{name}-examples
Summary:        Protocol framework for ZeroMQ
Requires:       %{name} = %{version}

%description -n %{name}-examples
This package contains examples to get started with zproto.

%files -n %{name}-examples
%defattr(-,root,root)
%{_mandir}/man3/*
%doc include/*
%doc src/clojure
%doc src/csharp
%doc src/go
%doc src/clojure
%doc src/php
%doc src/main
%doc src/python
%doc src/selftest
%doc src/test
%doc src/zproto_example*
%doc src/zyre_peer_example

%changelog
* Wed Dec 31 2014 zproto Developers <zeromq-dev@lists.zeromq.org
- Initial packaging.
