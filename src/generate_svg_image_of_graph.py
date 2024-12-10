import sys
import requests
import argparse
import json

def generate_graph_image(dot_code, output_file):
    try:
        # Format request body as JSON
        body = {
            'graph': dot_code,
            'layout': 'dot',
            'format': 'svg'
        }

        # Make the POST request
        response = requests.post(
            'https://quickchart.io/graphviz',
            json=body,  # This automatically sets Content-Type to application/json
            timeout=10
        )
        
        # Print response status and content for debugging
        print(f"Status code: {response.status_code}")
        
        # Check response
        if response.status_code == 200:
            with open(output_file, 'wb') as f:
                f.write(response.content)
            print(f"Graph image saved as {output_file}")
            return True
        else:
            print(f"API Error: {response.status_code}")
            print(f"Response: {response.text}")
            return False

    except Exception as e:
        print(f"Error: {str(e)}")
        import traceback
        traceback.print_exc()
        return False

def main():
    parser = argparse.ArgumentParser(description='Generate graph visualization')
    parser.add_argument('dot_file', help='Path to DOT file')
    parser.add_argument('-o', '--output', default='resources\\graph_image\\dfa_output.svg', help='Output file path')
    
    args = parser.parse_args()
    
    try:
        # Read DOT code from file
        with open(args.dot_file, 'r') as f:
            dot_code = f.read().strip()  # Remove any extra whitespace
        
        success = generate_graph_image(dot_code, args.output)
        sys.exit(0 if success else 1)
    except Exception as e:
        print(f"Error reading DOT file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()