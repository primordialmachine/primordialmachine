# Copy the contents of
# "./../../libraries/data-definition-language/documentation/sources"
# to
# "./data-definition-language"
Remove-Item "./data-definition-language" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/data-definition-language/documentation/sources" -Destination "./data-definition-language" -Recurse

# Copy the contents of
# "./../../libraries/asset-definition-language/documentation/sources"
# to
# "./asset-definition-language"
Remove-Item "./asset-definition-language" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/asset-definition-language/documentation/sources" -Destination "./asset-definition-language" -Recurse

# Copy the contents of
# "./../../libraries/core/documentation/sources"
# to
# "./core/ring-1"
Remove-Item "./core" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/core/documentation/sources" -Destination "./core" -Recurse

# Copy the contents of
# "./../../libraries/aal/documentation/sources"
# to
# "./aal"
Remove-Item "./aal" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/aal/documentation/sources" -Destination "./aal" -Recurse

# Copy the contents of
# "./../../libraries/aal/documentation/sources"
# to
# "./val"
Remove-Item "./val" -Force -Recurse -ErrorAction SilentlyContinue
Copy-Item "./../../libraries/val/documentation/sources" -Destination "./val" -Recurse
