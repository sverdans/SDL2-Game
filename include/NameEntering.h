#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <GameInitializer.h>
#include <GraphicElement.h>

class NameEntering final : public GraphicElement
{
private:

	template <class T>
	class List
	{
	private:
		class Node
		{
		public:
			Node* next;
			T data;

			Node(T data)
			{
				this->data = data;
				next = nullptr;
			}

			Node(T data, Node* next)
			{
				this->data = data;
				this->next = next;
			}
		};

		int nodeCount = 0;
		Node* head;

		Node* current;

	public:

		List()
		{
			head = nullptr;
			current = head;
			nodeCount = 0;
		}

		~List()
		{
			Node* temp = head;
			Node* buf;
			while (temp != nullptr)
			{
				buf = temp;
				temp = temp->next;
				delete buf;
			}
			delete temp;
		}

		Node* first_node()
		{
			current = head;
			return current;
		}

		Node* to_next_node()
		{
			current = current->next;
			return current;
		}

		void insert(int index, T data)
		{
			if (index < 0 || index > nodeCount)
				return;

			if (head == nullptr)
				head = new Node(data);

			else if (index == 0)
			{
				Node* buf = new Node(data, head);
				head = buf;
			}

			else if (index == nodeCount)
			{
				Node* temp = head;
				while (temp->next != nullptr)
					temp = temp->next;
				temp->next = new Node(data);
			}
			else
			{
				Node* temp = head;
				for (int i = 1; i < index; i++)
					temp = temp->next;
				Node* buf = new Node(data, temp->next);
				temp->next = buf;
			}
			nodeCount++;
		}

		void erase(int index)
		{
			if (index < 0 || index >= nodeCount)
				return;
			if (index == 0 && nodeCount == 1)
			{
				delete head;
				head = nullptr;
			}
			else if (index == 0)
			{
				Node* buf = head->next;
				delete head;
				head = buf;
			}
			else if (index == nodeCount - 1)
			{
				Node* temp = head;
				while (temp->next->next != nullptr)
					temp = temp->next;

				delete temp->next;
				temp->next = nullptr;
			}
			else
			{
				Node* temp = head;
				for (int i = 1; i < index; i++)
					temp = temp->next;
				Node* buf = temp->next;
				temp->next = temp->next->next;
				delete buf;
			}
			nodeCount--;
		}

		T& at(int index)
		{
			Node* temp = head;
			for (int i = 0; i < index; i++)
				temp = temp->next;
			return temp->data;
		}

		T& at(Node* cur) { return cur->data; }
	};

	int max_length;
	int letters_counter;
	int name_length = 0;
	int a;

	SDL_Texture* cursor;
	SDL_Rect cursor_in_file = { 0, 0, 11, 14 };
	SDL_Rect cursor_position;
	int cursor_pos;

	SDL_Texture* alphabet;
	List<char> name;
	List<SDL_Rect> letters_in_file;
	List<SDL_Rect> letters_position;
	SDL_Rect letter_position;
	int letter_height_shift;

	void draw_name()
	{
		SDL_RenderClear(renderer);

		if (cursor_pos == 0)
		{
			cursor_position.x = backgroundPosition.w / 258 + backgroundPosition.x + (backgroundPosition.w - name_length) / 2;
			if (cursor_pos != letters_counter)
				letters_position.at(cursor_pos).x = cursor_position.x + cursor_position.w;
			for (int i = cursor_pos + 1; i < letters_counter; i++)
				letters_position.at(i).x = letters_position.at(i - 1).x + letters_position.at(i - 1).w;
		}
		else if (letters_counter > 0)
		{
			int i;

			letters_position.at(0).x = backgroundPosition.w / 258 + backgroundPosition.x + (backgroundPosition.w - name_length) / 2;
			for (i = 1; i < cursor_pos; i++)
			{
				letters_position.at(i).x = letters_position.at(i - 1).x + letters_position.at(i - 1).w;
			}
			cursor_position.x = letters_position.at(i - 1).x + letters_position.at(i - 1).w;
			if (cursor_pos != letters_counter)
				letters_position.at(cursor_pos).x = cursor_position.x + cursor_position.w;
			for (i = cursor_pos + 1; i < letters_counter; i++)
				letters_position.at(i).x = letters_position.at(i - 1).x + letters_position.at(i - 1).w;
		}

		SDL_RenderCopy(renderer, background, &backgroundInFile, &backgroundPosition);

		auto file_node = letters_in_file.first_node();
		auto pos_node = letters_position.first_node();

		for (file_node; file_node != nullptr; file_node = letters_in_file.to_next_node(), pos_node = letters_position.to_next_node())
			SDL_RenderCopy(renderer, alphabet, &letters_in_file.at(file_node), &letters_position.at(pos_node));

		SDL_RenderCopy(renderer, cursor, &cursor_in_file, &cursor_position);
		SDL_RenderPresent(renderer);
	}

public:
	NameEntering(SDL_Renderer* renderer, int screen_width, int screen_height, int name_length)
	{
		max_length = name_length;
		this->renderer = renderer;
		cursor_pos = 0;

		surface = IMG_Load("textures/alphabet.png");
		alphabet = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/entering_name.png");
		background = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		surface = IMG_Load("textures/cursor.png");
		cursor = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		setWindowResolution(screen_width, screen_height);
	}

	~NameEntering()
	{
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(alphabet);
		SDL_DestroyTexture(cursor);
	}

	void setWindowResolution(int width, int height) override
	{
		calculateWindowChanges(width, height);

		name_length = 0;
		letter_height_shift = backgroundPosition.y + backgroundPosition.h * 155 / 322;

		cursor_position = { 0,
			letter_height_shift,
			cursor_in_file.w * backgroundPosition.w / 258,
			cursor_in_file.h * backgroundPosition.h / 322 };

		name_length += cursor_position.w;

		auto file_node = letters_in_file.first_node();
		auto pos_node = letters_position.first_node();

		for (file_node; file_node != nullptr; file_node = letters_in_file.to_next_node(), pos_node = letters_position.to_next_node())
		{
			letters_position.at(pos_node) = { 0,
				letter_height_shift,
				letters_in_file.at(file_node).w * backgroundPosition.w / 258,
				letters_in_file.at(file_node).h * backgroundPosition.h / 322 };

			name_length += letters_position.at(pos_node).w;
		}

		draw_name();
	}

	void add_letter(SDL_Rect letter_in_file, char letter)
	{
		if (letters_counter < max_length)
		{
			letters_counter++;
			name.insert(cursor_pos, letter);
			letters_in_file.insert(cursor_pos, letter_in_file);
			letter_position = { 0, letter_height_shift, letter_in_file.w * backgroundPosition.w / 258, letter_in_file.h * backgroundPosition.h / 322 };
			letters_position.insert(cursor_pos, letter_position);
			name_length += letter_position.w;
			cursor_pos++;
			draw_name();
		}
	}

	void delete_letter()
	{
		if (cursor_pos > 0)
		{
			if (letters_counter > 1)
			{
				letters_counter--;
				cursor_pos--;
				name_length -= letters_position.at(letters_counter).w;
				name.erase(cursor_pos);
				letters_in_file.erase(cursor_pos);
				letters_position.erase(cursor_pos);

				letters_position.at(0).x = backgroundPosition.x + (backgroundPosition.w - name_length) / 2;
				for (int i = 1; i < letters_counter; i++)
					letters_position.at(i).x = letters_position.at(i - 1).x + letters_position.at(i - 1).w;
				cursor_position.x = letters_position.at(letters_counter - 1).x + letters_position.at(letters_counter - 1).w;
			}
			else if (letters_counter == 1)
			{
				letters_counter--;
				cursor_pos--;
				name_length -= letters_position.at(0).w;
				name.erase(cursor_pos);
				letters_in_file.erase(cursor_pos);
				letters_position.erase(cursor_pos);
				cursor_position.x = backgroundPosition.x + (backgroundPosition.w - name_length) / 2;
			}
			draw_name();
		}
	}

	void cursor_left()
	{
		if (cursor_pos > 0)
			cursor_pos--;
		draw_name();
	}

	void cursor_right()
	{
		if (cursor_pos < letters_counter)
			cursor_pos++;
		draw_name();
	}

	bool get_name(char** nickname)
	{
		if (letters_counter == 0)
			return false;

		*nickname = new char[max_length];
		int i;
		auto node = name.first_node();

		for (i = 0; i < letters_counter; i++, node = name.to_next_node())
			*(*nickname + i) = name.at(node);

		for (i; i < max_length; i++)
			*(*nickname + i) = ' ';
		return true;
	}
};