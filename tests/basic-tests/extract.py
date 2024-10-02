import os
import json
from collections import defaultdict

root_dir = './' 

file_names = [
    "lind_mutex_16_x.json", 
    "lind_regular_16_x.json", 
    "lind_spin_16_x.json", 
    "lind_mutex_x_16.json", 
    "lind_regular_x_16.json", 
    "lind_spin_x_16.json", 
    "nat_regular_16_x.json", 
    "nat_regular_x_16.json",
    "lind_mutex_x_x.json",
    "lind_spin_x_x.json",
    "lind_regular_x_x.json",
    "nat_regular_x_x.json"
]

for file_name in file_names:
    merged_data = defaultdict(list)
    
    for folder in os.listdir(root_dir):
        folder_path = os.path.join(root_dir, folder, 'data')
        file_path = os.path.join(folder_path, file_name)
        
        if os.path.exists(file_path):
            with open(file_path, 'r') as f:
                try:
                    json_data = json.load(f)
                except json.JSONDecodeError:
                    print(f"Error decoding JSON in {file_path}")
                    continue
            
            for label, data in json_data.items():
                merged_data[label].append(data)
    
    output_file = os.path.join(root_dir, f"{file_name}")
    with open(output_file, 'w') as f:
        json.dump(merged_data, f, indent=4)

    print(f"Data from {file_name} merged successfully into {output_file}")
