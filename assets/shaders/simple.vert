
attribute vec4 vPosition;
attribute vec3 vNormal;

uniform mat4 modelViewMatrix;

varying vec4 vColor;

void main() {
  gl_Position = modelViewMatrix * vPosition;
  
  vColor = vec4(1.0, 0.0, 0.0, 1.0);
}