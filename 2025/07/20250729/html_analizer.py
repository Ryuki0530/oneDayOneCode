# html_analizer.py

class html_analizer:
    def __init__(self, html, debug=False):
        self.html = html
        self.debug = debug

    def set_html(self, html):
        self.html = html
        if self.debug:
            print("HTML content set for analysis.")

    def tag_count(self,):
        self.tag_counts = {}
        if self.debug:
            print("Analyzing HTML content...")
        for tag in self.html.split('<')[1:]:
            tag_name = tag.split('>')[0].split()[0]
            if tag_name.startswith('/'):
                continue
            if tag_name in self.tag_counts:
                self.tag_counts[tag_name] += 1
            else:
                self.tag_counts[tag_name] = 1
        return self.tag_counts
    
    def tag_hierarchy(self):
        if not self.html:
            if self.debug:
                print("No HTML content to analyze.")
            return []
        
        hierarchy = []
        tag_stack = []  # 開始タグのスタック
        
        if self.debug:
            print("Analyzing HTML hierarchy...")
        
        # HTMLを '<' で分割してタグを抽出
        parts = self.html.split('<')
        
        for part in parts[1:]:  # 最初の要素は '<' の前の部分なのでスキップ
            if '>' not in part:
                continue
                
            tag_content = part.split('>')[0]
            
            # 終了タグ処理
            if tag_content.startswith('/'):
                tag_name = tag_content[1:].strip().lower()
                if tag_stack and tag_stack[-1] == tag_name:
                    tag_stack.pop()
                continue
            
            # 自己完結タグ
            if tag_content.endswith('/'):
                tag_name = tag_content[:-1].split()[0].strip().lower()
                current_level = len(tag_stack)
                hierarchy.append((current_level, tag_name))
                continue
            
            # 通常の開始タグ
            tag_name = tag_content.split()[0].strip().lower()
            
            # 現在の階層を記録
            current_level = len(tag_stack)
            hierarchy.append((current_level, tag_name))
            
            # スタックに追加（終了タグがあるタグのみ）
            tag_stack.append(tag_name)
        
        if self.debug:
            print(f"Found {len(hierarchy)} tags in hierarchy")
        
        return hierarchy

    def get_hierarchy(self):
        hierarchy = self.tag_hierarchy()
        seen = set()
        unique_hierarchy = []
        
        for level, tag_name in hierarchy:
            key = (level, tag_name)
            if key not in seen:
                seen.add(key)
                unique_hierarchy.append((level, tag_name))
        
        return unique_hierarchy