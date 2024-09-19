# frozen_string_literal: true

source "https://rubygems.org"

gem "jekyll-theme-chirpy", "~> 6.5", ">= 6.5.3"

group :test do
  gem "html-proofer", "~> 4.4"
end

# Windows and JRuby does not include zoneinfo files, so bundle the tzinfo-data gem
# and associated library.
platforms :mingw, :x64_mingw, :mswin, :jruby do
  gem "tzinfo", ">= 1", "< 3"
  gem "tzinfo-data"
end

gem 'slim', '~> 3.0.7'
gem 'thread_safe', '~> 0.3.5'
group :jekyll_plugins do
  gem "jekyll-asciidoc"
end

# Performance-booster for watching directories on Windows
gem "wdm", "~> 0.1.1", :platforms => [:mingw, :x64_mingw, :mswin]

# Lock `http_parser.rb` gem to `v0.6.x` on JRuby builds since newer versions of the gem
# do not have a Java counterpart.
gem "http_parser.rb", "~> 0.6.0", :platforms => [:jruby]

# do not need by github actions but need by local serve
gem "webrick", "~> 1.8"
gem "json", "~> 2.7"
gem "asciidoctor-plantuml"
gem "erb", "~> 4.0"
