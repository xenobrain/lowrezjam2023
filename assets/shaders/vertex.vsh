        #version 330

        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 texcoord;

        out vec2 frag_texcoord;

        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(position, 0.0, 1.0);
            frag_texcoord = vec2(texcoord.x, 1.0 - texcoord.y);
        }