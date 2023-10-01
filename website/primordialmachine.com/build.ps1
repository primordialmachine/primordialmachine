# Copy the contents of
# "./../../libraries/data-definition-language/documentation/sources"
# to
# "./data-definition-language"
Remove-Item "./data-definition-language" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/data-definition-language/documentation/sources" -Destination "./data-definition-language" -Recurse