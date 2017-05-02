Name:           huff
Version:        1.0
Release:        1
Summary:        Huffman archiver based on standart algorithm

Group:          Applications/Tools
License:        BSD
URL:            None
Source0:        %name-%{version}.tar.gz

BuildRequires:  gcc

%description
A simple huffman archiver.

%prep
%setup -q %{name}-%{version}


%build
make all


%install
mkdir -p %{buildroot}/usr/local/bin
install huff %{buildroot}/usr/local/bin


%clean
make clean
rm -rf %{buildroot}
rm -rf %{_builddir}/%{name}-%{version}


%files
%defattr(-,root,root)
/usr/local/bin/huff


%changelog
* Tue May 1 2017 Anton Svech
- Initial buld
