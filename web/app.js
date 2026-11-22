// FridgeToFork Web Controller & WebAssembly Bridge (Team T141)
let catalog = [];
let selectedIngredients = new Set();
let currentFilter = 'all';
let wasmReady = false;

const FALLBACK_CATALOG = [
    {
        "id": 1,
        "title": "Classic Scrambled Eggs",
        "category": "Breakfast",
        "prep_time": 10,
        "ingredients": [
            "egg",
            "butter",
            "milk"
        ],
        "instructions": "Whisk eggs with a splash of milk. Melt butter in a non-stick pan over medium-low heat and stir gently until soft curds form."
    },
    {
        "id": 2,
        "title": "French Toast",
        "category": "Breakfast",
        "prep_time": 15,
        "ingredients": [
            "bread",
            "egg",
            "milk",
            "butter"
        ],
        "instructions": "Whisk egg and milk in a shallow dish. Dip bread slices on both sides and fry in melted butter until golden brown."
    },
    {
        "id": 3,
        "title": "Simple Grilled Cheese",
        "category": "Snack",
        "prep_time": 10,
        "ingredients": [
            "bread",
            "cheese",
            "butter"
        ],
        "instructions": "Place cheese between bread slices, butter the outside of both slices, and toast in a pan until golden and melted."
    },
    {
        "id": 4,
        "title": "Tomato Onion Omelette",
        "category": "Breakfast",
        "prep_time": 12,
        "ingredients": [
            "egg",
            "onion",
            "tomato",
            "butter"
        ],
        "instructions": "Finely chop onion and tomato, whisk with eggs, and cook in a hot buttered pan until set on both sides."
    },
    {
        "id": 5,
        "title": "Garlic Butter Pasta",
        "category": "Dinner",
        "prep_time": 20,
        "ingredients": [
            "pasta",
            "garlic",
            "butter",
            "cheese"
        ],
        "instructions": "Boil pasta al dente. Saut\u00e9 minced garlic in butter, toss with pasta, and fold in grated cheese."
    },
    {
        "id": 6,
        "title": "Jeera Fried Rice",
        "category": "Lunch",
        "prep_time": 15,
        "ingredients": [
            "rice",
            "butter",
            "onion",
            "cumin"
        ],
        "instructions": "Crackle/saut\u00e9 cumin and sliced onion in butter until golden. Add cooked rice and toss over high heat."
    },
    {
        "id": 7,
        "title": "Egg Fried Rice",
        "category": "Dinner",
        "prep_time": 15,
        "ingredients": [
            "rice",
            "egg",
            "onion",
            "garlic",
            "butter"
        ],
        "instructions": "Scramble eggs in a wok and set aside. Saut\u00e9 onion and garlic in butter, add cooked rice, and fold in eggs."
    },
    {
        "id": 8,
        "title": "Aloo Jeera (Spiced Potato)",
        "category": "Lunch",
        "prep_time": 20,
        "ingredients": [
            "potato",
            "onion",
            "garlic",
            "butter",
            "cumin"
        ],
        "instructions": "Boil and cube potato. Saut\u00e9 cumin, chopped onion, and garlic in butter, add potato cubes, and fry until crisp."
    },
    {
        "id": 9,
        "title": "Paneer Bhurji",
        "category": "Dinner",
        "prep_time": 20,
        "ingredients": [
            "paneer",
            "onion",
            "tomato",
            "garlic",
            "butter"
        ],
        "instructions": "Crumble paneer. Saut\u00e9 onion, garlic, and tomato in butter until soft. Fold in paneer and simmer for 5 minutes."
    },
    {
        "id": 10,
        "title": "Garlic Herb Bread",
        "category": "Snack",
        "prep_time": 12,
        "ingredients": [
            "bread",
            "garlic",
            "butter"
        ],
        "instructions": "Mix minced garlic with softened butter, spread generously over bread slices, and toast until crisp."
    },
    {
        "id": 11,
        "title": "Tomato Basil Pasta",
        "category": "Dinner",
        "prep_time": 25,
        "ingredients": [
            "pasta",
            "tomato",
            "garlic",
            "cheese",
            "butter"
        ],
        "instructions": "Simmer chopped tomato and minced garlic in butter until a rich sauce forms. Toss with boiled pasta and top with cheese."
    },
    {
        "id": 12,
        "title": "Creamy Banana Oatmeal",
        "category": "Breakfast",
        "prep_time": 10,
        "ingredients": [
            "oat",
            "milk",
            "banana",
            "honey"
        ],
        "instructions": "Simmer oat in milk for 5 minutes until creamy. Top with sliced banana and drizzle with honey."
    },
    {
        "id": 13,
        "title": "Banana Honey Smoothie",
        "category": "Beverage",
        "prep_time": 5,
        "ingredients": [
            "banana",
            "milk",
            "honey"
        ],
        "instructions": "Combine ripe banana, chilled milk, and honey in a blender and blend on high until smooth and frothy."
    },
    {
        "id": 14,
        "title": "Zesty Egg Salad",
        "category": "Lunch",
        "prep_time": 15,
        "ingredients": [
            "egg",
            "onion",
            "lemon"
        ],
        "instructions": "Hard boil egg and chop roughly. Toss with finely diced onion and fresh lemon juice."
    },
    {
        "id": 15,
        "title": "Potato Onion Frittata",
        "category": "Breakfast",
        "prep_time": 25,
        "ingredients": [
            "egg",
            "potato",
            "onion",
            "cheese",
            "butter"
        ],
        "instructions": "Saut\u00e9 thin potato slices and onion in butter until tender. Pour beaten egg over top, sprinkle cheese, and cook on low."
    },
    {
        "id": 16,
        "title": "Quick Palak Paneer",
        "category": "Dinner",
        "prep_time": 25,
        "ingredients": [
            "paneer",
            "spinach",
            "onion",
            "tomato",
            "garlic"
        ],
        "instructions": "Blanch and puree spinach. Saut\u00e9 onion, garlic, and tomato, stir in spinach puree and cubed paneer, and simmer 6 minutes."
    },
    {
        "id": 17,
        "title": "Chicken Garlic Stir-Fry",
        "category": "Dinner",
        "prep_time": 20,
        "ingredients": [
            "chicken breast",
            "garlic",
            "onion",
            "bell pepper",
            "butter"
        ],
        "instructions": "Slice chicken breast into strips and saut\u00e9 with garlic in butter. Add bell pepper and onion and stir-fry for 4 minutes."
    },
    {
        "id": 18,
        "title": "Vegetable Pulao",
        "category": "Lunch",
        "prep_time": 30,
        "ingredients": [
            "rice",
            "carrot",
            "onion",
            "potato",
            "butter"
        ],
        "instructions": "Saut\u00e9 sliced onion, diced carrot, and potato in butter. Add washed rice and water, cover, and steam for 15 minutes."
    },
    {
        "id": 19,
        "title": "Cheesy Potato Bake",
        "category": "Snack",
        "prep_time": 30,
        "ingredients": [
            "potato",
            "cheese",
            "milk",
            "butter",
            "garlic"
        ],
        "instructions": "Layer thinly sliced potato with minced garlic, butter, and milk. Top with cheese and bake until bubbling."
    },
    {
        "id": 20,
        "title": "Honey Butter Toast",
        "category": "Snack",
        "prep_time": 8,
        "ingredients": [
            "bread",
            "butter",
            "honey"
        ],
        "instructions": "Toast bread slices in butter until golden brown and drizzle warm honey over top immediately."
    },
    {
        "id": 21,
        "title": "Masala Cheese Toast",
        "category": "Snack",
        "prep_time": 12,
        "ingredients": [
            "bread",
            "cheese",
            "onion",
            "tomato",
            "butter"
        ],
        "instructions": "Top bread with finely diced onion, tomato, and grated cheese. Toast in butter on a covered skillet until cheese melts."
    },
    {
        "id": 22,
        "title": "Lemon Garlic Chicken",
        "category": "Dinner",
        "prep_time": 22,
        "ingredients": [
            "chicken breast",
            "lemon",
            "garlic",
            "butter"
        ],
        "instructions": "Pan-sear chicken breast in butter with minced garlic until golden. Deglaze pan with fresh lemon juice and spoon over top."
    },
    {
        "id": 23,
        "title": "Spinach Cheese Omelette",
        "category": "Breakfast",
        "prep_time": 10,
        "ingredients": [
            "egg",
            "spinach",
            "cheese",
            "butter"
        ],
        "instructions": "Wilt spinach in butter for 1 minute. Pour in whisked egg, sprinkle cheese in the center, and fold over."
    },
    {
        "id": 24,
        "title": "Creamy Garlic Mushroom Rice",
        "category": "Dinner",
        "prep_time": 20,
        "ingredients": [
            "rice",
            "mushroom",
            "garlic",
            "butter",
            "cheese"
        ],
        "instructions": "Saut\u00e9 sliced mushroom and minced garlic in butter until browned. Fold in warm rice and grated cheese."
    },
    {
        "id": 25,
        "title": "Cucumber Tomato Salad",
        "category": "Lunch",
        "prep_time": 8,
        "ingredients": [
            "cucumber",
            "tomato",
            "onion",
            "lemon"
        ],
        "instructions": "Dice cucumber, tomato, and onion into bite-sized pieces. Toss with fresh lemon juice and serve chilled."
    },
    {
        "id": 26,
        "title": "Paneer Tikka Skillet",
        "category": "Snack",
        "prep_time": 18,
        "ingredients": [
            "paneer",
            "bell pepper",
            "onion",
            "yogurt",
            "lemon"
        ],
        "instructions": "Marinate cubed paneer, bell pepper, and onion in spiced yogurt and lemon juice. Sear in a hot skillet until charred."
    },
    {
        "id": 27,
        "title": "Banana Pancake",
        "category": "Breakfast",
        "prep_time": 15,
        "ingredients": [
            "banana",
            "egg",
            "oat",
            "milk",
            "butter"
        ],
        "instructions": "Mash banana and blend with egg, oat, and milk into a smooth batter. Cook small pancakes in a buttered pan."
    },
    {
        "id": 28,
        "title": "Garlic Mashed Potato",
        "category": "Dinner",
        "prep_time": 25,
        "ingredients": [
            "potato",
            "butter",
            "milk",
            "garlic"
        ],
        "instructions": "Boil peeled potato until fork-tender. Mash with warm milk, melted butter, and roasted minced garlic until fluffy."
    },
    {
        "id": 29,
        "title": "Mushroom Cheese Toastie",
        "category": "Snack",
        "prep_time": 14,
        "ingredients": [
            "bread",
            "mushroom",
            "cheese",
            "butter",
            "garlic"
        ],
        "instructions": "Saut\u00e9 sliced mushroom with garlic in butter. Layer onto bread with cheese and grill until crisp."
    },
    {
        "id": 30,
        "title": "Curd Rice (Thayir Sadam)",
        "category": "Lunch",
        "prep_time": 10,
        "ingredients": [
            "rice",
            "yogurt",
            "milk",
            "cucumber"
        ],
        "instructions": "Mash warm cooked rice with yogurt and a splash of milk. Fold in finely diced cucumber and serve cool."
    }
];

async function initApp() {
    try {
        const response = await fetch('../data/recipes.json');
        if (!response.ok) throw new Error('HTTP ' + response.status);
        catalog = await response.json();
    } catch (err) {
        // Works seamlessly when opening index.html directly via file:// protocol
        catalog = FALLBACK_CATALOG;
    }

    // Check if Emscripten WebAssembly module is loaded and initialize its catalog
    if (typeof Module !== 'undefined' && typeof Module.initCatalog === 'function') {
        try {
            wasmReady = Module.initCatalog(JSON.stringify(catalog));
            document.getElementById('engineStatusText').textContent = 'C++ Wasm Engine Active (' + catalog.length + ' recipes)';
        } catch (e) {
            console.warn('Wasm init fallback:', e);
        }
    } else {
        document.getElementById('engineStatusText').textContent = 'Client Engine Ready (' + catalog.length + ' recipes)';
    }

    setupEventListeners();
    updateUI();
}

function normalizeIngredient(raw) {
    if (!raw) return "";
    let s = raw.trim().toLowerCase();
    if (s.length > 3 && s.endsWith("oes")) s = s.slice(0, -2);
    else if (s.length > 3 && s.endsWith("ies")) s = s.slice(0, -3) + "y";
    else if (s.length > 2 && s.endsWith("s") && !s.endsWith("ss")) s = s.slice(0, -1);
    return s;
}

// Single-call bridge to C++ WebAssembly or deterministic C++-mirror algorithm
function getRecommendations(userIngredientsList, topN = 10, minNearThreshold = 50.0) {
    if (wasmReady && typeof Module !== 'undefined' && typeof Module.recommendRecipes === 'function') {
        const jsonStr = Module.recommendRecipes(JSON.stringify(userIngredientsList), topN);
        return JSON.parse(jsonStr);
    }

    const userSet = new Set(userIngredientsList.map(normalizeIngredient));
    let results = [];

    catalog.forEach(recipe => {
        const req = recipe.ingredients.map(normalizeIngredient);
        const matched = req.filter(i => userSet.has(i));
        const missing = req.filter(i => !userSet.has(i));
        const matchPct = (matched.length / req.length) * 100.0;
        const isExact = (missing.length === 0);
        const isNear = (!isExact && missing.length <= 2 && matchPct >= minNearThreshold);

        if (isExact || isNear) {
            results.push({
                id: recipe.id,
                title: recipe.title,
                category: recipe.category,
                prep_time: recipe.prep_time,
                match_percentage: matchPct,
                is_exact: isExact,
                is_near: isNear,
                matched_ingredients: matched,
                missing_ingredients: missing,
                instructions: recipe.instructions
            });
        }
    });

    // Deterministic 4-Tier Tie-Breaker Ranking (M3)
    results.sort((a, b) => {
        if (Math.abs(a.match_percentage - b.match_percentage) > 0.001) {
            return b.match_percentage - a.match_percentage;
        }
        if (a.missing_ingredients.length !== b.missing_ingredients.length) {
            return a.missing_ingredients.length - b.missing_ingredients.length;
        }
        const aTotal = a.matched_ingredients.length + a.missing_ingredients.length;
        const bTotal = b.matched_ingredients.length + b.missing_ingredients.length;
        if (aTotal !== bTotal) return aTotal - bTotal;
        return a.title.localeCompare(b.title);
    });

    return results.slice(0, topN);
}

function setupEventListeners() {
    const input = document.getElementById('ingredientInput');
    const addBtn = document.getElementById('addBtn');
    const findBtn = document.getElementById('findRecipesBtn');
    const clearBtn = document.getElementById('clearAllBtn');

    addBtn.addEventListener('click', () => {
        const val = normalizeIngredient(input.value);
        if (val) {
            addIngredient(val);
            input.value = '';
        }
    });

    input.addEventListener('keydown', (e) => {
        if (e.key === 'Enter') {
            e.preventDefault();
            addBtn.click();
        }
    });

    document.getElementById('quickPills').addEventListener('click', (e) => {
        if (e.target.classList.contains('pill-btn')) {
            addIngredient(e.target.dataset.ingredient);
        }
    });

    clearBtn.addEventListener('click', () => {
        selectedIngredients.clear();
        updateUI();
        renderResults([]);
    });

    findBtn.addEventListener('click', executeSearch);

    document.getElementById('resultFilters').addEventListener('click', (e) => {
        if (e.target.classList.contains('filter-btn')) {
            document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
            e.target.classList.add('active');
            currentFilter = e.target.dataset.filter;
            if (selectedIngredients.size > 0) executeSearch();
        }
    });
}

function addIngredient(name) {
    const norm = normalizeIngredient(name);
    if (norm && !selectedIngredients.has(norm)) {
        selectedIngredients.add(norm);
        updateUI();
    }
}

function removeIngredient(name) {
    selectedIngredients.delete(name);
    updateUI();
}

function updateUI() {
    const container = document.getElementById('selectedContainer');
    const countSpan = document.getElementById('ingredientCount');
    countSpan.textContent = selectedIngredients.size;

    if (selectedIngredients.size === 0) {
        container.innerHTML = '<p class="empty-hint" id="emptyHint">No ingredients selected yet. Type above or click a quick-select staple!</p>';
        return;
    }

    container.innerHTML = Array.from(selectedIngredients).map(ing => `
        <span class="tag-chip">
            ${ing}
            <span class="tag-remove" onclick="removeIngredient('${ing}')">&times;</span>
        </span>
    `).join('');
}

function executeSearch() {
    if (selectedIngredients.size === 0) {
        alert("Please select at least one ingredient first!");
        return;
    }

    let results = getRecommendations(Array.from(selectedIngredients), 12);

    if (currentFilter === 'exact') results = results.filter(r => r.is_exact);
    if (currentFilter === 'near') results = results.filter(r => r.is_near);

    renderResults(results);
}

function renderResults(results) {
    const grid = document.getElementById('recipesGrid');
    if (results.length === 0) {
        grid.innerHTML = `
            <div style="grid-column: 1/-1; text-align: center; padding: 2rem; color: #64748b;">
                <p style="font-size: 1.1rem; font-weight: 600;">No qualifying recipes found (&gt;= 50% match &amp; &le; 2 missing).</p>
                <p style="font-size: 0.9rem;">Try adding common staples like butter, egg, milk, onion, or bread.</p>
            </div>
        `;
        return;
    }

    grid.innerHTML = results.map(r => `
        <article class="recipe-card">
            <div>
                <div class="card-top">
                    <h3 class="recipe-title">${r.title}</h3>
                    <span class="prep-badge">${r.prep_time}m &bull; ${r.category}</span>
                </div>
                <div class="match-badge ${r.is_exact ? 'badge-cook-now' : 'badge-near-match'}">
                    ${r.is_exact ? '★ Cook Now (100% Match)' : `✦ Near Match (${Math.round(r.match_percentage)}%)`}
                </div>
                <div class="ingredients-breakdown">
                    <div class="matched-list">✓ Have (${r.matched_ingredients.length}): ${r.matched_ingredients.join(', ')}</div>
                    ${r.missing_ingredients.length > 0 
                        ? `<div class="missing-list">✗ Missing (${r.missing_ingredients.length}): ${r.missing_ingredients.join(', ')}</div>` 
                        : ''}
                </div>
            </div>
            <p class="recipe-instructions">${r.instructions}</p>
        </article>
    `).join('');
}

window.addEventListener('DOMContentLoaded', initApp);
