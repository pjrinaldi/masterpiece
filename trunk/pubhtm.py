from docutils.core import publish_file


output = publish_file(source_path="./tmppub.tht", destination_path="./tmppub.htm", writer_name='html')
#output = publish_string(a, writer_name="html")
#print output