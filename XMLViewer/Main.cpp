# include <Siv3D.hpp>

class TreeView;

namespace
{
	constexpr float SCENE_WIDTH  = 800;
	constexpr float SCENE_HEIGHT = 600;
	constexpr Vec2  VIEW_OFFSET{ 10, 40 };
	constexpr RectF VIEW_RECT{ VIEW_OFFSET, Vec2{SCENE_WIDTH, SCENE_HEIGHT} - (VIEW_OFFSET + Vec2{10, 10}) };

	std::unique_ptr<XMLReader> reader{ nullptr };
	std::unique_ptr<TreeView>  root  { nullptr };
}

struct TreeItem
{
	String  item{ U"item" };
	Texture icon{ Icon(0xF01BC), 16 };

	TreeItem() = default;
	TreeItem(StringView name)
		: item{ name }
	{
	}
	TreeItem(StringView name, Texture i)
		: item{ name }
		, icon{ i }
	{
	}

	virtual RectF update(float x, float y) { return region(x, y); }

	virtual RectF draw(float x, float y) const
	{
		float itemDrawX = x + icon.width() * 1.1f;
		auto  iconRect = icon.draw(Vec2{ x, y + icon.height() * 0.5f });
		auto  itemRect = FontAsset(U"font16")(item).draw(Vec2{ itemDrawX, y });
		auto  retRect = iconRect;
		retRect.w *= 1.1f;
		retRect.w += itemRect.w;
		retRect.h = itemRect.bottomY() - y;
		if (retRect.mouseOver())
		{
			retRect.drawFrame();
		}
		return retRect;
	}

	RectF region(float x, float y) const
	{
		float itemDrawX = x + icon.width() * 1.1f;
		auto  iconRect = icon.region(Vec2{ x, y + icon.height() * 0.5f });
		auto  itemRect = FontAsset(U"font16")(item).region(Vec2{ itemDrawX, y });
		auto  retRect = iconRect;
		retRect.w *= 1.1f;
		retRect.w += itemRect.w;
		retRect.h = itemRect.bottomY() - y;
		return retRect;
	}
};

struct TreeView : public TreeItem
{
	TreeView()
		: TreeItem(U"tree", Texture{ Icon(0xF0770), 16 })
	{
	}

	TreeView(StringView name)
		: TreeItem(name, Texture{ Icon(0xF0770), 16 })
	{
	}

	~TreeView()
	{
		for (auto& child : children)
		{
			if (child)
			{
				delete child;
				child = nullptr;
			}
		}
	}

	void open()
	{
		isOpen = true;
		icon = TextureAsset(U"open");
	}

	void close()
	{
		isOpen = false;
		icon = TextureAsset(U"close");
	}

	virtual RectF update(float x, float y) override
	{
		auto itemRect = region(x, y);
		auto collision = itemRect;
		auto retRect = itemRect;
		if (isOpen)
		{
			float offsetX = x;
			float offsetY = y;
			float indent = icon.width() * 4.0f;
			offsetX += indent;
			for (const auto& child : children)
			{
				offsetY += itemRect.h * 1.1f;
				itemRect = child->update(offsetX, offsetY);
				if (retRect.rightX() < itemRect.rightX())
				{
					retRect.w += itemRect.rightX() - retRect.rightX();
				}
			}
		}
		retRect.h = itemRect.bottomY() - y;
		if (VIEW_RECT.mouseOver())
		{
			if (collision.mouseOver() and collision.leftClicked())
			{
				(isOpen ? close() : open());
			}
		}
		return retRect;
	}

	virtual RectF draw(float x, float y) const override
	{
		const auto& font = FontAsset(U"font16");
		auto   itemRect = TreeItem::draw(x, y);
		auto   retRect = itemRect;
		if (isOpen)
		{
			float offsetX = x;
			float offsetY = y;
			float indent = icon.width() * 4.0f;
			offsetX += indent;
			for (const auto& child : children)
			{
				offsetY += itemRect.h * 1.1f;
				itemRect = child->draw(offsetX, offsetY);
				if (retRect.rightX() < itemRect.rightX())
				{
					retRect.w += itemRect.rightX() - retRect.rightX();
				}
			}
		}
		retRect.h = itemRect.bottomY() - y;
		return retRect;
	}

	TreeView* CreateChildTree(StringView name)
	{
		TreeView* tree = new TreeView{ name };
		children.push_back(tree);
		return tree;
	}

	TreeItem* CreateChildItem(StringView name)
	{
		TreeItem* item = new TreeItem{ name };
		children.push_back(item);
		return item;
	}

	TreeItem* CreateChildItem(StringView name, Texture i)
	{
		TreeItem* item = new TreeItem{ name, i };
		children.push_back(item);
		return item;
	}

private:

	Array<TreeItem*> children{};
	bool isOpen{ true };

};

void addXMLElement(TreeView* view, const XMLElement& element)
{
	for (auto e = element.firstChild(); e; e = e.nextSibling())
	{
		auto tree = view->CreateChildTree(e.name());
		for (const auto& attribute : e.attributes())
		{
			tree->CreateChildItem(U"{}={}"_fmt(attribute.first, attribute.second), TextureAsset(U"attribute"));
		}
		addXMLElement(tree, e);
		if (auto text = e.text())
		{
			tree->CreateChildItem(text);
		}
	}
}

namespace CustomGUI
{
	constexpr ColorF BackgroundColor{ 1.0 };
	constexpr ColorF DisabledBackgroundColor{ 0.92 };
	constexpr ColorF ButtonMouseOverColor{ 0.9, 0.95, 1.0 };
	constexpr ColorF SliderBaseColor{ 0.33 };
	constexpr ColorF SliderFillColor{ 0.35, 0.7, 1.0 };
	constexpr ColorF SliderDisabledBaseColor{ 0.75 };
	constexpr ColorF SliderDisabledFillColor{ 0.75, 0.85, 1.0 };
	constexpr double SliderBarRoundSize = 2.0;
	constexpr double SliderMinLength = 40.0;

	bool VerticalSliderAt(double& value, double min, double max, const Vec2& center, const double _sliderHeight, const bool enabled)
	{
		const double sliderWidth = Max(_sliderHeight, SliderMinLength);
		const double height = sliderWidth;
		const RectF region{ Arg::center = center, 36, height };

		const double sliderRegionY0 = (region.y + 8);
		const double sliderRegionY1 = (region.y + region.h - 8);
		const double sliderRegionH = (sliderRegionY1 - sliderRegionY0);

		const double actualSliderRegionY0 = (sliderRegionY0 + 8);
		const double actualSliderRegionY1 = (sliderRegionY1 - 8);
		const double actualSliderRegionH = (actualSliderRegionY1 - actualSliderRegionY0);

		const RectF sliderRect{ Arg::topCenter(center.x, sliderRegionY0), 6, sliderRegionH };
		const RoundRect baseRoundRect = sliderRect.rounded(2);

		const double previousValue = value;
		const auto [min_, max_] = std::minmax(min, max);
		value = Clamp(value, min_, max_);

		const double fill = ((value - min) / (max - min));
		const RectF smallRect{ Arg::center(center.x, actualSliderRegionY1 - actualSliderRegionH * fill), 6, 16 };
		const RoundRect smallRoundRect = smallRect.rounded(4.2);
		const bool mouseOver = (enabled && smallRect.mouseOver());

		if (enabled)
		{
			baseRoundRect.draw(SliderBaseColor);
			smallRoundRect
				.draw(mouseOver ? ButtonMouseOverColor : BackgroundColor)
				.drawFrame(1, ColorF(0.33));
		}
		else
		{
			baseRoundRect.draw(SliderDisabledBaseColor);
			smallRoundRect
				.draw(DisabledBackgroundColor)
				.drawFrame(1, ColorF(0.67));
		}

		const RectF sliderRectExtended = sliderRect.stretched(12, 4);

		if (enabled && Cursor::OnClientRect() && (sliderRectExtended.mouseOver() || smallRect.mouseOver()))
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (enabled && Cursor::OnClientRect() && sliderRectExtended.leftPressed())
		{
			const double pos = (actualSliderRegionY1 - Cursor::PosF().y);
			const double posN = Math::Saturate(pos / actualSliderRegionH);
			value = (min + posN * (max - min));
		}

		return (value != previousValue);
	}

	bool SliderAt(double& value, double min, double max, const Vec2& center, const double _sliderWidth, const bool enabled)
	{
		const double sliderWidth = Max(_sliderWidth, SliderMinLength);
		const RectF region{ Arg::center = center, sliderWidth, 0.0 };

		const double sliderRegionX0 = (region.x + 8);
		const double sliderRegionX1 = (region.x + region.w - 8);
		const double sliderRegionW = (sliderRegionX1 - sliderRegionX0);

		const double actualSliderRegionX0 = (sliderRegionX0 + 8);
		const double actualSliderRegionX1 = (sliderRegionX1 - 8);
		const double actualSliderRegionW = (actualSliderRegionX1 - actualSliderRegionX0);

		const RectF sliderRect{ Arg::leftCenter(sliderRegionX0, center.y), sliderRegionW, 6 };
		const RoundRect baseRoundRect = sliderRect.rounded(SliderBarRoundSize);

		const double previousValue = value;
		const auto [min_, max_] = std::minmax(min, max);
		value = Clamp(value, min_, max_);

		const double fill = ((value - min) / (max - min));
		const RectF smallRect{ Arg::center(actualSliderRegionX0 + actualSliderRegionW * fill, center.y), 16, 6 };
		const RoundRect smallRoundRect = smallRect.rounded(4.2);
		const bool mouseOver = (enabled && smallRect.mouseOver());

		if (enabled)
		{
			baseRoundRect.draw(SliderBaseColor);
			smallRoundRect
				.draw(mouseOver ? ButtonMouseOverColor : BackgroundColor)
				.drawFrame(1, ColorF{ 0.33 });
		}
		else
		{
			baseRoundRect.draw(SliderDisabledBaseColor);
			smallRoundRect
				.draw(DisabledBackgroundColor)
				.drawFrame(1, ColorF{ 0.67 });
		}

		const RectF sliderRectExtended = sliderRect.stretched(4, 12);

		if (enabled && Cursor::OnClientRect() && (sliderRectExtended.mouseOver() || smallRect.mouseOver()))
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		if (enabled && Cursor::OnClientRect() && sliderRectExtended.leftPressed())
		{
			const double pos = (Cursor::PosF().x - actualSliderRegionX0);
			const double posN = Math::Saturate(pos / actualSliderRegionW);
			value = (min + posN * (max - min));
		}

		return (value != previousValue);
	}
}

namespace MenuFunc
{
	void OpenXMLFile(void)
	{
		if (auto filePath = Dialog::OpenFile({
				FileFilter {
					U"XML",
					{
						U"xml",
						U"vcxproj",
						U"sspj",
						U"ssce",
						U"ssae",
						U"ssee",
					}
				},
				FileFilter::AllFiles()
			}))
		{
			auto path = filePath.value();
			reader.reset(new XMLReader{ path });
			root.reset(new TreeView{ reader->name() });
			addXMLElement(root.get(), *reader);
		}
	}

	void AppExit(void)
	{
		System::Exit();
	}
}

void Main()
{
	Window::SetTitle(U"XMLViewer");
	Scene::Resize(SCENE_WIDTH, SCENE_HEIGHT);

	FontAsset::Register(U"font16", 16);
	TextureAsset::Register(U"open", Icon(0xF0770), 16);
	TextureAsset::Register(U"close", Icon(0xF024B), 16);
	TextureAsset::Register(U"attribute", Icon(0xF0174), 16);
	TextureAsset::Register(U"data", Icon(0xF01BC), 16);

	Vec2 scroll{ 0.0f, 0.0f };
	Vec2 maxScroll{ 0.0f, 0.0f };
	SimpleMenuBar menuBar{
		{
			{
				U"ファイル",
				{
					U"XMLファイルを開く",
					U"アプリの終了"
				}
			},
		}
	};
	const Array<Array<void(*)(void)>> MENU_FUNCTION_TABLE = {
		{
			MenuFunc::OpenXMLFile,
			MenuFunc::AppExit,
		},
	};

	Polygon maskPolygon{ Shape2D::Ngon(4, SCENE_WIDTH, Scene::CenterF(), 45_deg) };
	maskPolygon.addHole(VIEW_RECT);

	while (System::Update())
	{
		const Vec2 viewPos{ VIEW_OFFSET - scroll };
		if (auto itemIndex = menuBar.update())
		{
			auto item = itemIndex.value();
			MENU_FUNCTION_TABLE[item.menuIndex][item.itemIndex]();
		}
		else
		{
			if (root)
			{
				maxScroll = root->update(viewPos.x, viewPos.y).size + VIEW_OFFSET;
			}
			if (InRange(maxScroll.x, 0.0, (double)Scene::Width()))
			{
				maxScroll.x = 0.0f;
			}
			else
			{
				maxScroll.x -= Scene::Width();
			}
			if (InRange(maxScroll.y, 0.0, (double)Scene::Height()))
			{
				maxScroll.y = 0.0f;
			}
			else
			{
				maxScroll.y -= Scene::Height();
			}
		}
		if (root)
		{
			root->draw(viewPos.x, viewPos.y);
		}
		maskPolygon.draw(Palette::DefaultBackground);
		menuBar.draw();
		auto openRect = TextureAsset(U"open").draw(100, 10, Palette::DefaultLetterbox);
		auto tagRect  = FontAsset(U"font16")(U": tag").draw(100 + openRect.w * 1.25, 3, Palette::DefaultLetterbox);
		auto attrRect = TextureAsset(U"attribute").draw(100 + tagRect.w + (openRect.w) * 1.25 + 20, 10, Palette::DefaultLetterbox);
		auto attrStrRect = FontAsset(U"font16")(U": attribute").draw(100 + tagRect.w + (openRect.w + attrRect.w) * 1.25 + 20, 3, Palette::DefaultLetterbox);
		auto dataRect = TextureAsset(U"data").draw(100 + tagRect.w + attrStrRect.w + (openRect.w + attrRect.w) * 1.25 + 40, 10, Palette::DefaultLetterbox);
		auto dataStrRect = FontAsset(U"font16")(U": data").draw(100 + tagRect.w + attrStrRect.w + (openRect.w + attrRect.w + dataRect.w) * 1.25 + 40, 3, Palette::DefaultLetterbox);

		auto menuHeight = menuBar.MenuBarHeight + 10.0;
		auto scrollBarV = SimpleGUI::VerticalSliderRegion(Vec2{ 0.0, 0.0 }, (double)Scene::Height() - menuHeight);
		double rScrollY = maxScroll.y - scroll.y;
		if (CustomGUI::VerticalSliderAt(rScrollY, 0.0, maxScroll.y, Vec2{ Scene::Width() - 8.0 * 0.5, (Scene::Height() + menuHeight) * 0.5 }, scrollBarV.h, true))
		{
			scroll.y = maxScroll.y - rScrollY;
		}
		auto scrollBarH = SimpleGUI::SliderRegion(Vec2{ 0.0, 0.0 }, 0.0, (double)Scene::Width());
		CustomGUI::SliderAt(scroll.x, 0.0, maxScroll.x, Vec2{ (Scene::Width() - 8.0) * 0.5, Scene::Height() -8.0 * 0.5 }, scrollBarH.w, true);

		if (KeyShift.pressed())
		{
			scroll.x += Mouse::Wheel() * 8.0f;
			scroll.y += Mouse::WheelH() * 8.0f;
		}
		else
		{
			scroll.x += Mouse::WheelH() * 8.0f;
			scroll.y += Mouse::Wheel() * 8.0f;
		}
		scroll.x = Clamp(scroll.x, 0.0, maxScroll.x);
		scroll.y = Clamp(scroll.y, 0.0, maxScroll.y);
	}
}
