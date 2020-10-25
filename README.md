# kompactor

## Garbage Compactor 3263827

This will temper the shadow meshes of the alo files as this:
 - it will merge vertices if they share the same key {position - normal};
 - it will then remove any faces that contain two identical vertices.
  
