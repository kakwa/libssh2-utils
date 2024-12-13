%define pkgname @NAME@
%define _unitdir /usr/lib/systemd/system/

Name: %{pkgname}
Version: @VERSION@
Release: @RELEASE@%{?dist}
Source: %{pkgname}-%{version}.tar.gz
URL: @URL@ 
Vendor: @MAINTAINER@
License: @LICENSE@
Group: System/Servers
Summary: @SUMMARY@ 
BuildRoot: %{_tmppath}/%{pkgname}-%{zone}-%{version}-%{release}-build
#BuildArch: noarch
BuildRequires: cmake, libssh2-devel, gcc, g++
Requires: libssh2

%description
@DESCRIPTION@

%prep

%setup -q -n %{pkgname}-%{version}

%install

rm -rf $RPM_BUILD_ROOT
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make install DESTDIR=$RPM_BUILD_ROOT

%files
%defattr(644, root, root, 755)
%attr(755, root, root)/usr/bin/*

%changelog
* Wed Feb 01 2013 @MAINTAINER@ <@MAINTAINER_EMAIL@> 0.0.1-1
- initial Version initiale
