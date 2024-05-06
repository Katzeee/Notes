require "asciidoctor-plantuml"

Asciidoctor::PlantUml.configure do |conf|
  conf.url = "https://plantuml.cca.io"
  conf.encoding = 'deflate'
end